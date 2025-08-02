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
    class Property {
        public:
            Property(PROPERTY& app);
            bool LoadCustomization(PROPERTY& app);

        public:
            [[nodiscard]] const std::wstring& GetName() const { return m_name; }
            [[nodiscard]] DATATYPES GetDataType() const { return m_type; }

        private:
            std::wstring m_name;
            DATATYPES m_type{};
    };

    void to_json(nlohmann::json& j, const Property& p);
}
