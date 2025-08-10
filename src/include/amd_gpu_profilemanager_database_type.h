// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// ADL SDK
#include <adl_sdk_extra.h>


//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    enum class DatabaseType {
        System,
        User,
        OEM,
        File
    };

    constexpr  DatabaseType from_adl(ADL_AP_DATABASE val) {
        switch (val) {
            case ADL_AP_DATABASE__SYSTEM:
                return DatabaseType::System;
            case ADL_AP_DATABASE__USER:
                return DatabaseType::User;
            case ADL_AP_DATABASE__OEM:
                return DatabaseType::OEM;
            default:
                return DatabaseType::System;
        }
    }

    constexpr ADL_AP_DATABASE to_adl(DatabaseType val) {
        switch (val) {
            case DatabaseType::System:
                return ADL_AP_DATABASE__SYSTEM;
            case DatabaseType::User:
                return ADL_AP_DATABASE__USER;
            case DatabaseType::OEM:
                return ADL_AP_DATABASE__OEM;
            default:
                return ADL_AP_DATABASE__SYSTEM;
        }
    }
}
