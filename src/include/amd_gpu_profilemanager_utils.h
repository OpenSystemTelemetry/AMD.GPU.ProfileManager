// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <cstdint>
#include <span>
#include <string>
#include <vector>

// AMD ADL
#include <adl_sdk_extra.h>



//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    namespace Utils {
        std::span<const uint8_t> GetTable(const void* buf, const TABLES& tbl);
        std::wstring ReadString(const void* root, STRINGS& str);
        std::string ToUtf8(const std::wstring& wstr);
        std::wstring ToUtf16(const std::string& wstr);
        void RemoveSubstring(std::wstring& wstr, const std::vector<std::wstring>& whats);
    }
}
