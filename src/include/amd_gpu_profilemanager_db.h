// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <filesystem>
#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "nlohmann/json.hpp"

#include "amd_gpu_profilemanager_application.h"
#include "amd_gpu_profilemanager_area.h"
#include "amd_gpu_profilemanager_profile.h"


//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    class DB {
        public:
            friend void to_json(nlohmann::json& j, const DB& db);

            void Clear();
            bool LoadCustomization(const CUSTOMISATIONS& customization);
            bool LoadBinary(const std::filesystem::path& filepath);
            bool SaveJson(const std::filesystem::path& filepath) const;

            const std::optional<Application*> GetApplication(const std::wstring& designator) const;

            [[nodiscard]] std::vector<std::shared_ptr<Application>> GetApplications() const;
            
            const std::vector<Area>& GetAreas() const { return m_area; }
            const std::vector<Profile>& GetProfiles() const { return m_profiles; }
            const std::wstring& GetRelease() const { return m_release; }
        private:
            std::vector<Area>        m_area;
            std::vector<Profile>     m_profiles;
            std::map<std::wstring, std::shared_ptr<Application>> m_applications;
            std::wstring m_release;
    };

    void to_json(nlohmann::json& j, const DB& db);
}
