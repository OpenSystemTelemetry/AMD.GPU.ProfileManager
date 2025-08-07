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
#include "window_details.h"


//
// Public
//

namespace OST::AMD::GPU::ProfileManager {
    struct AppDisplayInfo {
        std::shared_ptr<OST::AMD::GPU::ProfileManager::Application> app;
        std::string m_designator;
        bool fsr_enabled;
    };


    class WindowMain: public Uni::GUI::UiElement {
        public:
            explicit WindowMain();
            ~WindowMain() override;
        
        // ADL
        private:
            static void* __stdcall adl_malloc ( int iSize );
        private:
            ADL_CONTEXT_HANDLE m_adl_context{};

        // DB
        private:
            void dbRefresh();
            void dbSave();
            static void dbSaveCallback(void* userdata, const char* const* files, int filte);
        private:
            std::map<ADL_AP_DATABASE, std::unique_ptr<OST::AMD::GPU::ProfileManager::DB>> m_db;
            std::map<ADL_AP_DATABASE, std::vector<AppDisplayInfo>> m_db_cache;
            bool m_db_cache_dirty{false};
            ADL_AP_DATABASE m_current_db_source = ADL_AP_DATABASE__SYSTEM;

        // UI
        public:
            bool UiUpdate() override;
            void uiUpdateTopbar();
            void uiUpdateTable();
            void uiUpdateModalCreateNewProfile();
            void uiUpdatePresetActions();
            bool isAppWhitelisted(const std::wstring& filename);
        private:
            char m_search_buffer[256]{};
            std::unique_ptr<WindowDetails> m_ui_details;
           std::wstring m_selected_preset;
    };
}
