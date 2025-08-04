// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// stdlib
#include <algorithm>
#include <string>
#include <locale>

// ImGUI
#include <imgui.h>

// GPU Profile Manager
#include "amd_gpu_profilemanager_utils.h"
#include "window_details.h"


//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    WindowDetails::WindowDetails(std::shared_ptr<OST::AMD::GPU::ProfileManager::Application> app) {
        m_adl_app = std::move(app);
    }

    WindowDetails::~WindowDetails() {}



    //
    // UI
    //

    bool WindowDetails::UiUpdate() {
        bool show{true};

        const auto* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize({viewport->Size.x*0.8f,viewport->Size.y*0.8f});
        ImGui::SetNextWindowPos({viewport->Size.x*0.1f,viewport->Size.y*0.1f});
        ImGui::OpenPopup("Details");
        if (ImGui::BeginPopupModal("Details", &show, ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::CollapsingHeader("Overview", ImGuiTreeNodeFlags_DefaultOpen)) {
                if(ImGui::BeginTable("Overview", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg )) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted("Title");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(Utils::ToUtf8(m_adl_app->GetTitle()).c_str());


                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted("File name");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(Utils::ToUtf8(m_adl_app->GetFileName()).c_str());

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("File path");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(Utils::ToUtf8(m_adl_app->GetPath()).c_str());


                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted("Version");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(Utils::ToUtf8(m_adl_app->GetVersion()).c_str());

                    ImGui::EndTable();
                }
            }

            ImGui::NewLine();

            if (ImGui::CollapsingHeader("Uses", ImGuiTreeNodeFlags_DefaultOpen)) {
                if(ImGui::BeginTable("Uses", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg )) {
                    ImGui::TableSetupColumn("Driver component");
                    ImGui::TableSetupColumn("Profile");
                    ImGui::TableHeadersRow();

                    const auto& uses = m_adl_app->GetUses();
                    for (size_t idx = 0; idx < uses.size(); idx++) {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%ls", uses[idx].GetDriver().c_str());
                        ImGui::TableNextColumn();
                        ImGui::Text("%ls", uses[idx].GetName().c_str());
                    }

                    ImGui::EndTable();
                }
            }
            ImGui::EndPopup();
        }

        return show;
    }
}
