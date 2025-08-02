// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
// 

// stdlib
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
// ADL SDK
#include <adl_sdk_extra.h>



//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    class Use {
        public:
            Use() = default;
            Use(const USE& app);
            bool LoadCustomization(const USE& app);

            [[nodiscard]] const std::wstring& GetDriver() const { return m_driver; }
            [[nodiscard]] const std::wstring& GetName() const { return m_name; }

        private:
            std::wstring m_driver;
            std::wstring m_name;
    };

    void to_json(nlohmann::json& j, const Use& p);
}
