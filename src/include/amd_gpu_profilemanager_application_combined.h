// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <string>

// nlohmann
#include <nlohmann/json.hpp>

// ADL SDK
#include <adl_sdk_extra.h>

// AMD GPU Profile Manager
#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_use.h"



//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    class ApplicationCombined {
    public:
        ApplicationCombined() = default;

        // Application
    public:
        void ApplicationAdd(std::shared_ptr<Application> app);
        void ApplicationAdd(ApplicationCombined& app_combined);
        std::vector<std::shared_ptr<Application>>& ApplicationGet();
    private:
        std::vector<std::shared_ptr<Application>> m_applications;

        // Info
    public:
        [[nodiscard]] std::wstring InfoDesignatorGet() const;
        [[nodiscard]] const std::vector<std::wstring>& InfoFilenameGet() const;
        [[nodiscard]] const std::vector<std::wstring>& InfoTitleGet() const;
        [[nodiscard]] bool InfoFsrEnabled() const;
    private:
        void infoRefresh();
    private:
        std::vector<std::wstring> m_info_filename{};
        std::wstring m_info_designator{};
        std::vector<std::wstring> m_info_title{};
        bool m_info_fsr_enabled{};

        // Profile
    public:
        bool ProfileAssign(ADL_CONTEXT_HANDLE context, const std::wstring& driver_area, const std::wstring& profile_name);

        // Remove
    public:
        bool Remove(ADL_CONTEXT_HANDLE context);

    };

    void to_json(nlohmann::json& j, const ApplicationCombined& a);
}
