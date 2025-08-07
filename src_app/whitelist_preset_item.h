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



//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    struct WhitelistPresetItem {
        std::wstring title;
        std::wstring exename;
    };
}
