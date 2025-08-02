// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
// 

// stdlib
#include <map>
#include <string>
#include <vector>

// ADL SDK
#include <adl_sdk_extra.h>

#include <nlohmann/json.hpp>

//
// Class
//

namespace OST::AMD::GPU::ProfileManager {
    class Profile {
        public:
            Profile(PROFILE& prof);
            bool LoadCustomization(PROFILE& prof);

            [[nodiscard]] const std::wstring& GetName() const { return m_name; }
            [[nodiscard]] const std::wstring& GetNote() const { return m_note; }
            [[nodiscard]] const std::map<std::wstring, std::wstring>& GetValues() const { return m_values; }

        private:
            std::wstring m_name;
            std::wstring m_note;
            std::map<std::wstring, std::wstring> m_values;
    };

    void to_json(nlohmann::json& j, const Profile& p);
}
