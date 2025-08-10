// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// stdlib
#include <codecvt>
#include <locale>

// AMD GPU Profile Manager
#include "amd_gpu_profilemanager_utils.h"



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

        void RemoveSubstring(std::wstring& wstr, const std::vector<std::wstring>& whats) {
            // Collect non-empty patterns
            std::vector<std::wstring_view> substrs;
            substrs.reserve(whats.size());
            for (const auto& w : whats) {
                if (!w.empty()) {
                    substrs.emplace_back(w);
                }
            }

            if (substrs.empty() || wstr.empty()) {
                return;
            }

            std::wstring::size_type pos = 0;
            while (pos < wstr.size()) {
                std::wstring::size_type best_pos = std::wstring::npos;
                std::wstring::size_type best_len = 0;

                for (auto substr : substrs) {
                    const auto found = wstr.find(substr, pos);
                    if (found == std::wstring::npos) {
                        continue;
                    }

                    if (found < best_pos || (found == best_pos && substr.size() > best_len)) {
                        best_pos = found;
                        best_len = substr.size();
                    }
                }

                if (best_pos == std::wstring::npos) {
                    break;
                }

                wstr.erase(best_pos, best_len);
                pos = best_pos;
            }
        }
    }
}
