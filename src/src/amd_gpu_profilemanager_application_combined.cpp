// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// AMD GPU Profile Manager
#include "amd_gpu_profilemanager_application_combined.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    //
    // Application
    //

    void ApplicationCombined::ApplicationAdd(std::shared_ptr<Application> app) {
        m_applications.push_back(app);
        infoRefresh();
    }

    void ApplicationCombined::ApplicationAdd(ApplicationCombined &app_combined) {
        for (auto& app: app_combined.m_applications) {
            ApplicationAdd(app);
        }
        infoRefresh();
    }

    std::vector<std::shared_ptr<Application>> & ApplicationCombined::ApplicationGet() {
        return m_applications;
    }


    //
    // Info
    //

    std::wstring ApplicationCombined::InfoDesignatorGet() const {
        return m_info_designator;
    }

    const std::vector<std::wstring>& ApplicationCombined::InfoFilenameGet() const {
        return m_info_filename;
    }

    const std::vector<std::wstring>& ApplicationCombined::InfoTitleGet() const {
        return m_info_title;
    }

    bool ApplicationCombined::InfoFsrEnabled() const {
        return m_info_fsr_enabled;
    }

    void ApplicationCombined::infoRefresh() {
        m_info_designator.clear();
        m_info_filename.clear();
        m_info_title.clear();
        m_info_fsr_enabled = false;

        if (m_applications.empty()) {
            return;
        }

        for (auto& app: m_applications) {
            // title
            {
                auto title = app->GetTitle();
                Utils::RemoveSubstring(title, {L"-DX11",L"-DX12", L" 64bit"});
                if (m_info_title.empty() || title.size() > m_info_title.front().size()) {
                    m_info_title.clear();
                    m_info_title.push_back(title);
                }
            }

            // filename
            auto app_filename = app->GetFileName();
            std::transform(app_filename.begin(), app_filename.end(), app_filename.begin(), ::towlower);

            bool contains{false};
            for (auto& f : m_info_filename) {
                if (f == app_filename) {
                    contains = true;
                    break;
                }
            }

            if (!contains){
                m_info_filename.push_back(app_filename);
            }

            // fsr
            for (const auto& use: app->GetUses()) {
                if (use.GetDriver() == L"FSROVR" && use.GetName() == L"FsrOvrWhitelistProfile") {
                    m_info_fsr_enabled = true;
                }
            }

            // designator
            m_info_designator += app->GetFileName();
            m_info_designator += L"|";
            m_info_designator += app->GetTitle();
            m_info_designator += L"|";
        }

        std::transform(m_info_designator.begin(), m_info_designator.end(), m_info_designator.begin(), ::towlower);
    }



    //
    // Profile
    //

    bool ApplicationCombined::ProfileAssign(ADL_CONTEXT_HANDLE context, const std::wstring &driver_area,
                                            const std::wstring &profile_name) {
        bool result{false};
        for (auto& app: m_applications) {
            result = app->AssignProfile(context, driver_area, profile_name) || result;
        }
        return result;
    }



    //
    // Remove
    //

    bool ApplicationCombined::Remove(ADL_CONTEXT_HANDLE context) {
        for (auto& app: m_applications) {
            app->Remove(context);
        }
        m_applications.clear();
        return true;
    }
}
