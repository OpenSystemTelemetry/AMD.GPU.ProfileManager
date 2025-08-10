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
#include "amd_gpu_profilemanager_database_type.h"
#include "window_details.h"


//
// Public
//

namespace OST::AMD::GPU::ProfileManager {
    class WindowMain: public Uni::GUI::UiElement {
        public:
            explicit WindowMain();
            ~WindowMain() override;
        
        // ADL

        private:
            ADL_CONTEXT_HANDLE m_adl_context{};

        // DB
        private:
            void dbRefresh();
            void dbSave();
            static void dbSaveCallback(void* userdata, const char* const* files, int filte);
        private:
            std::map<DatabaseType, DB> m_db;
            std::map<DatabaseType, std::vector<ApplicationCombined>> m_db_apps;
            std::map<DatabaseType, bool> m_db_dirty;
            DatabaseType m_db_selected{DatabaseType::System};
            DatabaseType m_db_selected_next{DatabaseType::System};

        // Search
            void searchUpdate();
        private:
            std::array<char, 256U> m_search_buf{};
            std::wstring m_search_str{};

        // UI
        public:
            bool UiUpdate() override;
            void uiUpdateNoAdl();
            void uiUpdateDbFailed();
            void uiUpdateTopbar();
            void uiUpdateTopbarSearch();
            void uiUpdateTable();
            void uiUpdateModalCreateNewProfile();
            void uiUpdatePresetActions();
        private:
            std::unique_ptr<WindowDetails> m_ui_details;
           std::wstring m_selected_preset;

    };
}
