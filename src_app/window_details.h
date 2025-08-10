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
        explicit WindowDetails(ApplicationCombined& app);
        ~WindowDetails() override;

        // Application
    private:
        ApplicationCombined& m_app;

        // UI
    public:
        bool UiUpdate() override;
        void UiUpdateApp(std::shared_ptr<Application> app);
    };
}
