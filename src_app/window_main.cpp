// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// stdlib
#include <algorithm>
#include <fstream>
#include <locale>
#include <map>
#include <string>

// SDL
#include <SDL3/SDL_dialog.h>

// ImGUI
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

// Nlohmann
#include <nlohmann/json.hpp>

// GPU Profile Manager
#include "window_main.h"
#include "amd_gpu_profilemanager_db.h"
#include "amd_gpu_profilemanager_utils.h"
#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_profile.h"
#include "amd_gpu_profilemanager_property.h"
#include "amd_gpu_profilemanager_use.h"
#include "whitelist_preset_manager.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    WindowMain::WindowMain() {
        if (ADL2_Main_Control_Create(adl_malloc, 1, &m_adl_context) != ADL_OK)
        {
            m_adl_context = nullptr;
        }

        for (const auto& type : {DatabaseType::System, DatabaseType::User, DatabaseType::OEM, DatabaseType::File}) {
            m_db[type] = std::make_unique<DB>();
            m_db_apps = {};
            m_db_dirty[type] = true;
        }

        WhitelistPresetManager::RegisterAll();
        dbRefresh();
    }

    WindowMain::~WindowMain() {
        if (m_adl_context) {
            ADL2_Main_Control_Destroy(m_adl_context);
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
        for (auto& [db_type, db_dirty] : m_db_dirty) {
            if (db_dirty){
                auto& db = m_db[db_type];
                db->Clear();
                CUSTOMISATIONS customisations{};
                if (m_adl_context) {
                    if (ADL2_ApplicationProfiles_GetCustomization(m_adl_context, to_adl(db_type), &customisations) == ADL_OK) {
                        db->LoadCustomization(customisations);
                    }
                }
                m_db_apps[db_type] = db->GetApplicationsCombined();
            }
            db_dirty = false;
        }
    }

    void WindowMain::dbSaveCallback(void* userdata, const char* const* files, int filte) {
        if (!files[0]) {
            return;
        }
        auto* cls = reinterpret_cast<WindowMain*>(userdata);
        auto filepath = std::filesystem::path(files[0]);
        filepath.replace_extension("json");
        cls->m_db[cls->m_db_selected]->SaveJson(filepath);
    }

    void WindowMain::dbSave() {
        SDL_Window* sdl_window = (SDL_Window*)ImGui::GetMainViewport()->PlatformHandle;
        SDL_DialogFileFilter filters[1] = { { "JSON", "json" } };
        SDL_ShowSaveFileDialog(WindowMain::dbSaveCallback, this, sdl_window, filters, 1, nullptr);
    }

    //
    // Search
    //

    void WindowMain::searchUpdate() {
        m_search_str = Utils::ToUtf16(m_search_buf.data());
        std::transform(m_search_str.begin(), m_search_str.end(),m_search_str.begin(),::towlower);
    }


    //
    // UI
    //

    bool WindowMain::UiUpdate() {
        m_db_selected = m_db_selected_next;
        dbRefresh();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
        if (ImGui::Begin("Database Viewer", nullptr, window_flags)) {
            uiUpdateTopbar();
            uiUpdateTable();
            uiUpdateModalCreateNewProfile();
            uiUpdatePresetActions();
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
                for (auto& [db_type, db_dirty] : m_db_dirty) {
                    db_dirty = true;
                }
            }

            if (ImGui::Button("Export")) {
                dbSave();
            }

            ImGui::Separator();

            if (ImGui::RadioButton("System", m_db_selected == DatabaseType::System)) {
                m_db_selected_next = DatabaseType::System;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("User", m_db_selected == DatabaseType::User)) {
                m_db_selected_next = DatabaseType::User;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("OEM", m_db_selected == DatabaseType::OEM)) {
                m_db_selected_next = DatabaseType::OEM;
            }
#if 0
            ImGui::SameLine();
            if (ImGui::RadioButton("File", m_db_selected == DatabaseType::File)) {
                m_db_selected_next = DatabaseType::File;
            }
#endif

            ImGui::SameLine();
            ImGui::Separator();
            ImGui::SameLine();

            const auto& db = m_db.at(m_db_selected);
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
            uiUpdateTopbarSearch();
            ImGui::EndMenuBar();
        }
    }

    void WindowMain::uiUpdateTopbarSearch() {
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 350);
        if (ImGui::InputTextWithHint("##Search", "Search...", m_search_buf.data(),  m_search_buf.size())) {
            searchUpdate();
        }
    }


    void WindowMain::uiUpdateTable() {
        if (ImGui::BeginTable("Applications",
            4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg ))
        {
            ImGui::TableSetupColumn("Title");
            ImGui::TableSetupColumn("Filename");
            ImGui::TableSetupColumn("FSR Whitelisted");
            ImGui::TableSetupColumn("Actions");
            ImGui::TableHeadersRow();

            // caching values since they could change in cycle
            auto& cur_db = m_db_apps[m_db_selected];
            auto cur_source = m_db_selected;

            size_t idx = 0;
            for (auto& app : cur_db) {
                auto app_designator = app.InfoDesignatorGet();
                auto app_fsr_enabled = app.InfoFsrEnabled();

                // Filter
                if (!m_search_str.empty()) {
                    if (app_designator.find(m_search_str) == std::string::npos) {
                        continue;
                    }
                }

                ImGui::TableNextRow();
                ImGui::PushID(idx++);

                if (app_fsr_enabled) {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0,
                        ImGui::ColorConvertFloat4ToU32(ImVec4(.4, .9, .0, .5)));
                }

                ImGui::TableNextColumn();
                for (const auto& filename : app.InfoTitleGet()) {
                    ImGui::Text("%ls", filename.c_str());
                }

                ImGui::TableNextColumn();
                for (const auto& filename : app.InfoFilenameGet()) {
                    ImGui::Text("%ls", filename.c_str());
                }

                ImGui::TableNextColumn();
                ImGui::Text("%ls", app_fsr_enabled ? L"YES" : L"NO");

                ImGui::TableNextColumn();


                ImGui::BeginDisabled(app_fsr_enabled);
                if (ImGui::Button("Whitelist FSR4")) {
                    app.ProfileAssign(m_adl_context, L"FSROVR", L"FsrOvrWhitelistProfile");
                    m_db_selected_next = DatabaseType::User;
                    m_db_dirty[m_db_selected_next] = true;
                }
                ImGui::EndDisabled();

                ImGui::SameLine();
                if (ImGui::Button("Details")) {
                    m_ui_details = std::make_unique<WindowDetails>(app);
                }

                ImGui::SameLine();
                ImGui::BeginDisabled(cur_source != DatabaseType::User);
                if (ImGui::Button("Remove")) {
                    app.Remove(m_adl_context);
                    m_db_dirty[m_db_selected] = true;
                }
                ImGui::EndDisabled();

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }

    void WindowMain::uiUpdatePresetActions() {
        if (m_db_selected != DatabaseType::User) {
            return;
        }

        const auto& presets = WhitelistPresetManager::GetAll();
        for (const auto& [preset_name, preset_apps] : presets) {
            ImGui::SameLine();
            std::string button_label = "Add " + Utils::ToUtf8(preset_name);

            if (ImGui::Button(button_label.c_str())) {
                m_selected_preset = preset_name;
                for (const auto& preset_app : preset_apps) {
                    Application app(preset_app.exename, preset_app.title, L"", L"");
                    app.AssignProfile(m_adl_context, L"FSROVR", L"FsrOvrWhitelistProfile");
                }
                m_db_dirty[m_db_selected] = true;
            }
        }
    }

    void WindowMain::uiUpdateModalCreateNewProfile() {
        if (m_db_selected == DatabaseType::User) {
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

            ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
            ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
            ImGui::InputTextWithHint("Path", "optional", path, IM_ARRAYSIZE(path));
            ImGui::InputTextWithHint("Version", "optional", version, IM_ARRAYSIZE(version));

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                std::wstring w_filename = Utils::ToUtf16(filename);
                std::wstring w_title = Utils::ToUtf16(title);
                std::wstring w_path = Utils::ToUtf16(path);
                std::wstring w_version = Utils::ToUtf16(version);

                Application app(w_filename, w_title, w_path, w_version);
                app.AssignProfile(m_adl_context, L"FSROVR", L"FsrOvrWhitelistProfile");

                m_db_dirty[m_db_selected] = true;
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
