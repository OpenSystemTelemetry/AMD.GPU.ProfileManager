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

#include "amd_gpu_profilemanager_use.h"

//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    class Application {
        public:
            Application() = default;
            Application(const APPLICATION& app);
            Application(const std::wstring& filename, const std::wstring& title, const std::wstring& path, const std::wstring& version);
            bool LoadCustomization(const APPLICATION& app);

            [[nodiscard]] const std::wstring GetDesignator() const { return m_designator; }
            [[nodiscard]] const std::wstring& GetFileName() const { return m_filename; }
            [[nodiscard]] const std::wstring& GetPath() const { return m_path; }
            [[nodiscard]] const std::wstring& GetTitle() const { return m_title; }
            [[nodiscard]] const std::wstring& GetVersion() const { return m_version; }
            [[nodiscard]] const std::vector<Use>& GetUses() const { return m_uses; }

            bool Remove(ADL_CONTEXT_HANDLE context);
            bool AssignProfile(ADL_CONTEXT_HANDLE context, const std::wstring& driver_area, const std::wstring& profile_name);

        private:
            std::wstring m_filename;
            std::wstring m_path;
            std::wstring m_title;
            std::wstring m_version;
            std::wstring m_designator;
            std::vector<Use> m_uses;
    };

    void to_json(nlohmann::json& j, const Application& a);
}
