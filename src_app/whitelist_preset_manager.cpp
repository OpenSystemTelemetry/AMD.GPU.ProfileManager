// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// amd gpu profile manager
#include "whitelist_preset_manager.h"



//
// Implementation
//
 
namespace OST::AMD::GPU::ProfileManager {
    namespace WhitelistPresetManager {
        static std::map<std::wstring, std::vector<WhitelistPresetItem>> m_data;

        const std::map<std::wstring, std::vector<WhitelistPresetItem>>& GetAll() {
            return m_data;
        }

        void Register(const std::wstring& title, const std::vector<WhitelistPresetItem>& apps) {
            m_data[title] =apps;
        }
    }
}
