// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
// 

// stdlib
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
// ADL SDK
#include <adl_sdk_extra.h>

#include "amd_gpu_profilemanager_property.h"


//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    class Area {
        public:
            Area(AREA& app);
            bool LoadCustomization(AREA& app);

        public:
            [[nodiscard]] const std::wstring& GetDriverName() const { return m_drivername; }
            [[nodiscard]] PRIVACYTYPES GetPrivacy() const { return m_privacy; }
            [[nodiscard]] const std::vector<Property>& GetProperties() const { return m_properties; }

        private:
           std::wstring m_drivername;
           PRIVACYTYPES m_privacy;

           std::vector<Property> m_properties;
    };

    void to_json(nlohmann::json& j, const Area& a);
}
