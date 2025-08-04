// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// stdlib
#include <algorithm>
#include <string>
#include <map>
#include <locale>

#include <fstream>

// SDL
#include <SDL3/SDL_dialog.h>

// ImGUI
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

// GPU Profile Manager
#include "window_main.h" // Primary header for this implementation file
#include "amd_gpu_profilemanager_db.h"
#include "amd_gpu_profilemanager_utils.h"
#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_profile.h"
#include "amd_gpu_profilemanager_property.h"
#include "amd_gpu_profilemanager_use.h"
#include <nlohmann/json.hpp>



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    WindowMain::WindowMain() {
        m_db[ADL_AP_DATABASE__SYSTEM] = std::make_unique<DB>();
        m_db[ADL_AP_DATABASE__OEM] = std::make_unique<DB>();
        m_db[ADL_AP_DATABASE__USER] = std::make_unique<DB>();
        if (ADL2_Main_Control_Create(adl_malloc, 1, &m_adl_context) != ADL_OK)
        {
            m_adl_context = nullptr;
        }

        m_db_cache_dirty = true;
        dbRefresh();
    }

    WindowMain::~WindowMain() {
        if (m_adl_context) {
            if (ADL2_Main_Control_Destroy(m_adl_context) != ADL_OK)
            {
                // Handle error
            }
        }
    }

    void* __stdcall WindowMain::adl_malloc ( int iSize )
    {
        void* lpBuffer = malloc ( iSize );
        return lpBuffer;
    }




    //
    // DB
    //

    void WindowMain::dbRefresh() {
        if (m_db_cache_dirty) {
            for (auto const& [db_type, db_ptr] : m_db) {
                db_ptr->Clear();
                CUSTOMISATIONS customisations{};
                if (m_adl_context) {
                    if (ADL2_ApplicationProfiles_GetCustomization(m_adl_context, db_type, &customisations) == ADL_OK) {
                        db_ptr->LoadCustomization(customisations);
                    }
                }
                m_db_cache[db_type].clear();

                for (const auto& app : db_ptr->GetApplications()) {
                    bool fsr_enabled = false;
                    for (const auto &use: app->GetUses()) {
                        if (use.GetDriver() == L"FSROVR" && use.GetName() == L"FsrOvrWhitelistProfile") {
                            fsr_enabled = true;
                            break;
                        }
                    }


                    auto designator = Utils::ToUtf8(app->GetDesignator());
                    std::transform(designator.begin(), designator.end(), designator.begin(), ::tolower);
                    m_db_cache[db_type].push_back({app, designator, fsr_enabled});
                }
            }
        }
        m_db_cache_dirty = false;
    }

    void WindowMain::dbSaveCallback(void* userdata, const char* const* files, int filte) {
        if (!files[0]) {
            return;
        }
        auto* cls = reinterpret_cast<WindowMain*>(userdata);
        auto filepath = std::filesystem::path(files[0]);
        filepath.replace_extension("json");
        cls->m_db[cls->m_current_db_source]->SaveJson(filepath);
    }

    void WindowMain::dbSave() {
        SDL_Window* sdl_window = (SDL_Window*)ImGui::GetMainViewport()->PlatformHandle;
        SDL_DialogFileFilter filters[1] = { { "JSON", "json" } };
        SDL_ShowSaveFileDialog(WindowMain::dbSaveCallback, this, sdl_window, filters, 1, nullptr);
    }


    //
    // UI
    //

    bool WindowMain::UiUpdate() {
        dbRefresh();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
        if (ImGui::Begin("Database Viewer", nullptr, window_flags)) {
            uiUpdateTopbar();
            uiUpdateTable();
            uiUpdateModalCreateNewProfile();
        }

        ImGui::End();

        if (m_ui_details) {
            if (!m_ui_details->UiUpdate()) {
                m_ui_details.reset();
            }
        }

        return true;
    }

    void WindowMain::uiUpdateTopbar() {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::Button("Refresh")) {
                m_db_cache_dirty = true;
                dbRefresh();
            }

           if (ImGui::Button("Export")) {
               dbSave();
           }

            ImGui::Separator();

            if (ImGui::RadioButton("System", m_current_db_source == ADL_AP_DATABASE__SYSTEM)) {
                m_current_db_source = ADL_AP_DATABASE__SYSTEM;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("User", m_current_db_source == ADL_AP_DATABASE__USER)) {
                m_current_db_source = ADL_AP_DATABASE__USER;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("OEM", m_current_db_source == ADL_AP_DATABASE__OEM)) {
                m_current_db_source = ADL_AP_DATABASE__OEM;
            }

            ImGui::SameLine();
            ImGui::Separator();
            ImGui::SameLine();

            const auto& db = m_db.at(m_current_db_source);
            if (db)
            {
                auto release = Utils::ToUtf8(db->GetRelease());
                if (!release.empty())
                {
                    ImGui::Text("Release: %s", release.c_str());
                }
            }

            // Search bar
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 350);
            if (ImGui::InputTextWithHint("##Search", "Search...", m_search_buffer, sizeof(m_search_buffer))) {
                // Nothing to do here, the filtering happens in uiUpdateTable
            }

            // Clear button
            if (m_search_buffer[0] != '\0') {
                ImGui::SameLine();
                if (ImGui::Button("X")) {
                    m_search_buffer[0] = '\0';
                }
            }

            ImGui::EndMenuBar();
        }
    }

    void WindowMain::uiUpdateTable() {
        if (ImGui::BeginTable("Applications",
            4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            ImGui::TableSetupColumn("Filename");
            ImGui::TableSetupColumn("Title");
            ImGui::TableSetupColumn("FSR Whitelisted");
            ImGui::TableSetupColumn("Actions");
            ImGui::TableHeadersRow();


            if (const ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
            {
                if (sort_specs->SpecsDirty)
                {
                    // Sort the data right now
                    std::sort(m_db_cache[m_current_db_source].begin(),
                        m_db_cache[m_current_db_source].end(),
                        [&](const auto& a, const auto& b) {
                            for (int i = 0; i < sort_specs->SpecsCount; i++)
                            {
                                const ImGuiTableColumnSortSpecs* column_spec = &sort_specs->Specs[i];

                                // Sort ascending or descending based on the current spec
                                int delta = 0;
                                switch (column_spec->ColumnIndex)
                                {
                                case 0: // Filename
                                        delta = a.app->GetFileName().compare(b.app->GetFileName());
                                        break;
                                    case 1: // Title
                                        delta = a.app->GetTitle().compare(b.app->GetTitle());
                                        break;
                                    case 2: // FSR Enabled
                                        delta = (a.fsr_enabled - b.fsr_enabled);
                                        break;
                                    default:
                                        break;
                                    }

                                    if (delta != 0)
                                    {
                                        if (column_spec->SortDirection == ImGuiSortDirection_Ascending) {
                                            return delta < 0;
                                        }
                                        return delta > 0;
                                    }
                                }
                                return false;
                            });
                    const_cast<ImGuiTableSortSpecs*>(sort_specs)->SpecsDirty = false;
                }
            }

            // caching values since they could change in cycle
            auto& cur_db = m_db_cache[m_current_db_source];
            auto cur_source = m_current_db_source;

            for (const auto& app : cur_db) {
                // Filter
                if (m_search_buffer[0] != '\0') {
                    std::string m_search_str = m_search_buffer;
                    std::transform(m_search_str.begin(), m_search_str.end(), m_search_str.begin(), ::tolower);
                    if (app.m_designator.find(m_search_buffer) == std::string::npos) {
                        continue;
                    }
                }

                ImGui::TableNextRow();
                ImGui::PushID(app.m_designator.c_str());

                if (app.fsr_enabled) {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0,
                        ImGui::ColorConvertFloat4ToU32(ImVec4(.4, .9, .0, .5)));
                }

                ImGui::TableNextColumn();
                ImGui::Text("%ls", app.app->GetFileName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%ls", app.app->GetTitle().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%ls", app.fsr_enabled ? L"YES" : L"NO");

                ImGui::TableNextColumn();


                ImGui::BeginDisabled(app.fsr_enabled);
                if (ImGui::Button("Whitelist FSR4")) {
                    m_current_db_source = ADL_AP_DATABASE__USER;
                    app.app->AssignProfile(m_adl_context, L"FSROVR", L"FsrOvrWhitelistProfile");
                    m_db_cache_dirty = true;
                }
                ImGui::EndDisabled();

                ImGui::SameLine();
                if (ImGui::Button("Details")) {
                    m_ui_details = std::make_unique<WindowDetails>(app.app);
                }

                ImGui::SameLine();
                ImGui::BeginDisabled(cur_source != ADL_AP_DATABASE__USER);
                if (ImGui::Button("Remove")) {
                    app.app->Remove(m_adl_context);
                    m_db_cache_dirty = true;
                }
                ImGui::EndDisabled();

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }

    void WindowMain::uiUpdateModalCreateNewProfile() {
        if (m_current_db_source == ADL_AP_DATABASE__USER) {
            if (ImGui::Button("Create")) {
                ImGui::OpenPopup("Create New Profile");
           }
       }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Create New Profile", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char filename[128] = "";
            static char title[128] = "";
            static char path[128] = "";
            static char version[128] = "";

            ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
            ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
            ImGui::InputText("Path", path, IM_ARRAYSIZE(path));
            ImGui::InputText("Version", version, IM_ARRAYSIZE(version));

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                std::wstring w_filename = Utils::ToUtf16(filename);
                std::wstring w_title = Utils::ToUtf16(title);
                std::wstring w_path = Utils::ToUtf16(path);
                std::wstring w_version = Utils::ToUtf16(version);

                Application app(w_filename, w_title, w_path, w_version);
                app.AssignProfile(m_adl_context, L"FSROVR", L"FsrOvrWhitelistProfile");

                m_db_cache_dirty = true;
                dbRefresh();

                ImGui::CloseCurrentPopup();
           }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
           }
            ImGui::EndPopup();
       }
   }
}
