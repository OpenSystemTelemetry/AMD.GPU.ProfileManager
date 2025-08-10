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
    WindowDetails::WindowDetails(ApplicationCombined& app): m_app(app) {
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
            int idx = 0;
            for (auto& app: m_app.ApplicationGet()) {
                ImGui::PushID(idx++);
                if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent();
                    UiUpdateApp(app);
                    ImGui::Unindent();
                }
                ImGui::PopID();
            }

            ImGui::EndPopup();
        }

        return show;
    }

    void WindowDetails::UiUpdateApp(std::shared_ptr<Application> m_adl_app) {
        if (ImGui::TreeNodeEx("Overview", ImGuiTreeNodeFlags_DefaultOpen)){
            if (!m_adl_app->GetFileName().empty()) {
                ImGui::TreeNodeEx("Title", ImGuiTreeNodeFlags_Leaf, "Title: %ls", m_adl_app->GetTitle().c_str());
                ImGui::TreePop();
            }

            if (!m_adl_app->GetFileName().empty()) {
                ImGui::TreeNodeEx("File name", ImGuiTreeNodeFlags_Leaf, "File name: %ls", m_adl_app->GetFileName().c_str());
                ImGui::TreePop();
            }

            if (!m_adl_app->GetPath().empty()) {
                ImGui::TreeNodeEx("File path", ImGuiTreeNodeFlags_Leaf, "File path: %ls", m_adl_app->GetPath().c_str());
                ImGui::TreePop();
            }

            if (!m_adl_app->GetVersion().empty()) {
                ImGui::TreeNodeEx("Version", ImGuiTreeNodeFlags_Leaf, "Version: %ls", m_adl_app->GetVersion().c_str());
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Uses", ImGuiTreeNodeFlags_DefaultOpen)){
            const auto& uses = m_adl_app->GetUses();
            for (size_t idx = 0; idx < uses.size(); idx++) {
                ImGui::TreeNodeEx("use", ImGuiTreeNodeFlags_Leaf, "%ls - %ls", uses[idx].GetDriver().c_str(), uses[idx].GetName().c_str());
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }
}
