// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

#include "amd_gpu_profilemanager_use.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    Use::Use(const USE& use){
        LoadCustomization(use);
    }

    bool Use::LoadCustomization(const USE& use){
        m_driver = use.DriverOfTheProfileToUse->NameOfDriver;
        m_name = use.NameOfTheProfileToUse;
        return true;
    }

    void to_json(nlohmann::json& j, const Use& p) {
        j = nlohmann::json{
            {"driver", Utils::ToUtf8(p.GetDriver())},
            {"name"  , Utils::ToUtf8(p.GetName())},
        };
    }
}
