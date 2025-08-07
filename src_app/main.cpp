// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//


// Uni.GUI
#include "ui_element.h"

// GPU Profile Manager
#include "whitelist_preset_manager.h"
#include "window_main.h"

//
// Implementation
//

std::string uni_gui_app_name_get()
{
    return "AMD GPU Profile Manager";
}

std::string uni_gui_app_version_get()
{
    return "0.0.4";
}

std::vector<std::shared_ptr<Uni::GUI::UiElement>> uni_gui_app_initialize( int argc, char **argv)
{
    OST::AMD::GPU::ProfileManager::WhitelistPresetManager::RegisterAll();
    return {std::make_shared<OST::AMD::GPU::ProfileManager::WindowMain>()};
}