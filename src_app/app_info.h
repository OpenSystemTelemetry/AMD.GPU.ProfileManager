// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <memory>
#include <string>

// gpu profile manager
#include "amd_gpu_profilemanager_application.h"



//
// Class
//
namespace OST::AMD::GPU::ProfileManager {
    struct AppDisplayInfo {
        std::shared_ptr<OST::AMD::GPU::ProfileManager::Application> app;
        std::string m_designator;
        bool fsr_enabled;
    };
}
