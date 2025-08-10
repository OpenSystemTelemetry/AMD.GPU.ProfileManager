// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <filesystem>
#include <vector>
#include <memory>

// nlohmann
#include "nlohmann/json.hpp"

// AMD GPU Profile Manager
#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_application_combined.h"
#include "amd_gpu_profilemanager_area.h"
#include "amd_gpu_profilemanager_database_type.h"
#include "amd_gpu_profilemanager_profile.h"


//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    class DB {
        public:
            static DB from_adl(ADL_CONTEXT_HANDLE handle, DatabaseType type);

            friend void to_json(nlohmann::json& j, const DB& db);

            void Clear();
            bool LoadCustomization(const CUSTOMISATIONS& customization);
            bool LoadBinary(const std::filesystem::path& filepath);
            bool SaveJson(const std::filesystem::path& filepath) const;

            [[nodiscard]] std::vector<std::shared_ptr<Application>> GetApplications() const;
            [[nodiscard]] std::vector<ApplicationCombined> GetApplicationsCombined() const;

            const std::vector<Area>& GetAreas() const { return m_area; }
            const std::vector<Profile>& GetProfiles() const { return m_profiles; }
            const std::wstring& GetRelease() const { return m_release; }
        private:
            std::vector<Area>        m_area;
            std::vector<Profile>     m_profiles;
            std::vector<std::shared_ptr<Application>> m_applications;
            std::wstring m_release;
    };

    void to_json(nlohmann::json& j, const DB& db);
}
