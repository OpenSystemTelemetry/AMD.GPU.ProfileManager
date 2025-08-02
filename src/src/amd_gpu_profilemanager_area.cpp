// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// magic_enum
#include <magic_enum/magic_enum.hpp>

// gpu profile manager
#include "amd_gpu_profilemanager_area.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    Area::Area(AREA& area){
        LoadCustomization(area);
    }

    bool Area::LoadCustomization(AREA& area){
        m_drivername = area.DriverComponent->NameOfDriver;
        m_privacy =area.DriverComponent->Privacy;

        auto* prop = area.HeadProperty;
        while(prop){
            m_properties.emplace_back(*prop);
            prop = prop->NextProperty;
        }

        return true;
    }

    void to_json(nlohmann::json& j, const Area& a) {
        j = nlohmann::json{
            {"driver_name", Utils::ToUtf8(a.GetDriverName())},
            {"privacy"    , magic_enum::enum_name<PRIVACYTYPES>(a.GetPrivacy())},
            {"properties" , a.GetProperties()},
        };
    }
}
