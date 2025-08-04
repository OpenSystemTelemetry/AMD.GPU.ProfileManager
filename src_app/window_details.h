// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <map>
#include <memory>
#include <vector>

// ADL
#include <adl_sdk_extra.h>

// Uni.GUI
#include "ui_element.h"

// GPU Profile Manager
#include "amd_gpu_profilemanager_db.h"
#include "amd_gpu_profilemanager_application.h"



//
// Public
//

namespace OST::AMD::GPU::ProfileManager {
    class WindowDetails: public Uni::GUI::UiElement {
    public:
        explicit WindowDetails(std::shared_ptr<OST::AMD::GPU::ProfileManager::Application> app);
        ~WindowDetails() override;

        // ADL
    private:
        std::shared_ptr<OST::AMD::GPU::ProfileManager::Application> m_adl_app;

        // UI
    public:
        bool UiUpdate() override;
    };
}
