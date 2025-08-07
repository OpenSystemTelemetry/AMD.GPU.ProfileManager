// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <string>

#include <vector>
#include <map>
 
// nlohmann
#include <nlohmann/json.hpp>
 
// amd gpu profile manager
#include "whitelist_preset_item.h"
 
//
// Class
//
 
namespace OST::AMD::GPU::ProfileManager {
    namespace WhitelistPresetManager{
        const std::map<std::wstring, std::vector<WhitelistPresetItem>>& GetAll();
        void Register(const std::wstring& title, const std::vector<WhitelistPresetItem>& apps);
        void RegisterAll();
    }
}
