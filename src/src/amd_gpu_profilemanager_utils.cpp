// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

#include "amd_gpu_profilemanager_utils.h"

#include <locale>
#include <codecvt>

//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    namespace Utils {
        std::span<const uint8_t> GetTable(const void* buf, const TABLES& tbl){
            const auto* base = reinterpret_cast<const uint8_t*>(buf);
            return {&base[tbl.Offset], tbl.Length};
        }

        std::wstring ReadString(const void* root, STRINGS& str){
            if(!root){
                return {};
            }

            std::wstring result;
            result.resize(str.Length/2);
            std::memcpy(result.data(), &reinterpret_cast<const uint8_t*>(root)[str.Offset * sizeof(uint64_t)], str.Length);
            return result;
        }

        std::string ToUtf8(const std::wstring& wstr){
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.to_bytes(wstr);
        }

        std::wstring ToUtf16(const std::string &str) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(str);
        }
    }
}
