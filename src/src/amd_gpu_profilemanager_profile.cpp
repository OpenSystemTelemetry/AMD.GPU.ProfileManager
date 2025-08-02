// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

#include "amd_gpu_profilemanager_profile.h"
#include "amd_gpu_profilemanager_utils.h"



//
// Implementation
//

namespace OST::AMD::GPU::ProfileManager {
    Profile::Profile(PROFILE& profile){
        LoadCustomization(profile);
    }

    bool Profile::LoadCustomization(PROFILE& profile){
        m_name = profile.NameOfThisProfile;
        if (profile.NotesAboutThisProfile) {
            m_note = profile.NotesAboutThisProfile;
        }

        auto* val = profile.HeadValue;
        while(val){
            std::wstring val_wstr;
            val_wstr.resize(val->iLengthOfValue/2);
            std::memcpy(val_wstr.data(), val->DataOfValue, val->iLengthOfValue);
            
            m_values[val->PropertyOfValue->NameOfProperty] = val_wstr;

            val = val->NextValue;
        }

        return true;
    }

    void to_json(nlohmann::json& j, const Profile& p) {
        j = nlohmann::json{
            {"name", Utils::ToUtf8(p.GetName())}
        };
        
        if(!p.GetNote().empty()){
            j["note"] = Utils::ToUtf8(p.GetNote());
        };

        const auto& vals = p.GetValues();
        if(!vals.empty()){
            auto val_obj = nlohmann::json{};
            for(const auto& kvp: vals){
                if(kvp.first == L"Dx11Blob"){
                    val_obj["Dx11Blob"] = "BLOB";
                }
                else {
                    val_obj[Utils::ToUtf8(kvp.first)] = Utils::ToUtf8(kvp.second);
                }
            }
            j["values"] = val_obj;
        }
    }
}
