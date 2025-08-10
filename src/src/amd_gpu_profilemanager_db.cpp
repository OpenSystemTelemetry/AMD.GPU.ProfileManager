// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// stdlib
#include <fstream>

// AMD ADL SDK
#include <adl_sdk_extra_structures.h>

// nlohmann
#include <nlohmann/json.hpp>

// OST AMD GPU ProfileManager
#include "amd_gpu_profilemanager_db.h"

#include <iostream>

#include "amd_gpu_profilemanager_utils.h"


//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    void DB::Clear() {
        m_area.clear();
        m_applications.clear();
    }

    bool DB::LoadCustomization(const CUSTOMISATIONS& customization){
        Clear();

        if(std::wstring(customization.Format) != L"1"){
            return false;
        }

        m_release = customization.Release;

        auto* area = customization.HeadArea;
        while(area){
            m_area.emplace_back(*area);
            area = area->NextArea;
        }

        auto* profile = customization.HeadProfile;
        while(profile){
            m_profiles.emplace_back(*profile);
            profile = profile->NextProfile;
        }

        auto* app_ptr = customization.HeadApplication;
        while(app_ptr){
            auto app_obj = std::make_unique<Application>(*app_ptr);
            m_applications.push_back(std::move(app_obj));
            app_ptr = app_ptr->NextApplication;
        }

        return true;
    }

    bool DB::LoadBinary(const std::filesystem::path& filepath){
        if(!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)){
            return false;
        }

        std::vector<uint8_t> filecontent(std::filesystem::file_size(filepath));

        {
            std::ifstream stream(filepath, std::ios::binary);
            stream.read(reinterpret_cast<char*>(filecontent.data()), filecontent.size());
        }

        auto* binfile = reinterpret_cast<BINFILE*>(filecontent.data());

        if(std::memcmp(binfile->Header.PreAmble, "BWJE", 4U) != 0){
            return false;
        }

        if(binfile->Header.Version != 1U){
            return false;
        }

        m_release = Utils::ReadString(&binfile->Body.Buffer, binfile->Header.Release);
    
        return true;
    }

    bool DB::SaveJson(const std::filesystem::path& filepath) const {
        nlohmann::json j = *this;

        std::ofstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        file << j.dump(4);
        return true;
    }

    std::vector<std::shared_ptr<Application>> DB::GetApplications() const {
        return m_applications;
    }

    std::vector<ApplicationCombined> DB::GetApplicationsCombined() const {
        // consolidate by exe name
        std::map<std::wstring, ApplicationCombined> combined_filename;
        for (const auto& app : m_applications) {
            auto filename = app->GetFileName();
            std::transform(filename.begin(),filename.end(), filename.begin(), ::towlower);
            if (!combined_filename.contains(filename)) {
                combined_filename[filename];
            }
            combined_filename[filename].ApplicationAdd(app);
        }

        // consolidate by title
        std::map<std::wstring, ApplicationCombined> combined_title;
        for (auto& pair : combined_filename) {
            auto title = pair.second.InfoTitleGet().front();
            std::ranges::transform(title, title.begin(), ::towlower);
            std::erase_if(title, ::iswspace);
            Utils::RemoveSubstring(title, {L"-dx11",L"-dx12", L" 64bit"});

            OutputDebugStringW(title.c_str());
            OutputDebugStringW(L"\r\n");

            if (!combined_title.contains(title)) {
                combined_title[title];
            }
            combined_title[title].ApplicationAdd(pair.second);
        }

        std::vector<ApplicationCombined> result;
        for (auto& pair : combined_title) {
            result.emplace_back(std::move(pair.second));
        }
        return result;
    }

    void to_json(nlohmann::json& j, const DB& db) {
        j = nlohmann::json{
            {"release", Utils::ToUtf8(db.m_release)},
            {"areas", db.m_area},
            {"profiles", db.m_profiles}
        };

        nlohmann::json apps_json = nlohmann::json::array();
        for (const auto& app : db.m_applications) {
            apps_json.push_back(*app);
        }
        j["applications"] = apps_json;
    }
}
