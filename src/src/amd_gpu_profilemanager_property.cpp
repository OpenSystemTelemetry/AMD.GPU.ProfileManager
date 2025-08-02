// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// magic_enum
#include <magic_enum/magic_enum.hpp>

// gpu profiler manager
#include "amd_gpu_profilemanager_property.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    Property::Property(PROPERTY& property){
        LoadCustomization(property);
    }

    bool Property::LoadCustomization(PROPERTY& prop){
        m_name = prop.NameOfProperty;
        m_type = prop.DataType;
        return true;
    }

    void to_json(nlohmann::json& j, const Property& p) {
        j = nlohmann::json{
            {"name", Utils::ToUtf8(p.GetName())},
            {"type", magic_enum::enum_name(p.GetDataType())},
        };
    }
}
