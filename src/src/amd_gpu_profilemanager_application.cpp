// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Inlcudes
//

#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    Application::Application(const APPLICATION& app){
        LoadCustomization(app);
    }

   Application::Application(const std::wstring& filename, const std::wstring& title, const std::wstring& path, const std::wstring& version) : m_filename(filename), m_title(title), m_path(path), m_version(version)
   {
   }

    bool Application::LoadCustomization(const APPLICATION& app){
        m_filename = app.FilenameOfThisApplication;
        m_title = app.TitleOfApplicationRecord;
        m_path = app.PartialPathOfApplication;
        m_version = app.Version;

        const auto* use = app.HeadUse;
        while(use){
            m_uses.emplace_back(*use);
            use = use->NextUse;
        }

        return true;
    }

    bool Application::Remove(ADL_CONTEXT_HANDLE context) {
        bool result = true;
        for(auto& use : m_uses) {
            result = ADL2_ApplicationProfiles_RemoveApplication(context, m_filename.c_str(), m_path.c_str(), m_version.c_str(), use.GetDriver().c_str()) == ADL_OK && result;
        }
        return result;
    }

    bool Application::AssignProfile(ADL_CONTEXT_HANDLE context, const std::wstring& driver_area, const std::wstring& profile_name) {
        bool result = true;
        result = ADL2_ApplicationProfiles_ProfileApplicationX2_Assign(context, m_filename.c_str(), m_path.c_str(), m_version.c_str(), m_title.c_str(), driver_area.c_str(), profile_name.c_str()) != ADL_OK;
        return result;
    }

    void to_json(nlohmann::json& j, const Application& a){
        j = nlohmann::json();
        
        if(!a.GetFileName().empty()){
            j["filename"] = Utils::ToUtf8(a.GetFileName());
        }

        if(!a.GetPath().empty()){
            j["path"] = Utils::ToUtf8(a.GetPath());
        }

        if(!a.GetTitle().empty()){
            j["title"] = Utils::ToUtf8(a.GetTitle());
        }

        if(!a.GetVersion().empty()){
            j["version"] = Utils::ToUtf8(a.GetVersion());
        }

        if(!a.GetUses().empty()){
            j["uses"] = a.GetUses();
        }
    }
}
