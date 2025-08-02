// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

//
// Includes
//

// ADL SDK Extra
#include "adl_sdk_extra.h"


//
// HELPERSs
//

static void* get_function(const char* fname){
    static HINSTANCE dll_handle = NULL;
    if(!dll_handle){
        dll_handle = LoadLibraryA("atiadlxx.dll");
    }
    if (!dll_handle){
        dll_handle = LoadLibraryA("atiadlxy.dll");
    }
    if(!dll_handle){
        return NULL;
    }

    return GetProcAddress(dll_handle, fname);
}



//
// Functions/Main
//

int ADL2_Main_Control_Create(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADL_CONTEXT_HANDLE *context){
    static ADL2_MAIN_CONTROL_CREATE f = NULL;
    if(!f){
        f = get_function("ADL2_Main_Control_Create");
    }
    if(!f){
        return INT_MIN;
    }
    return f(callback, iEnumConnectedAdapters, context);
}

int ADL2_Main_Control_Destroy(ADL_CONTEXT_HANDLE context){
    static ADL2_MAIN_CONTROL_DESTROY f = NULL;
    if(!f){
        f = get_function("ADL2_Main_Control_Destroy");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context);
}



//
// Functions/ApplicationProfile
//

int ADL2_ApplicationProfiles_Applications_Get(ADL_CONTEXT_HANDLE context, const wchar_t* driverArea, int* numApps, ADLApplicationRecord** adlApplications) {
    static ADL2_APPLICATIONPROFILES_APPLICATIONS_GET f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_Applications_Get");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, driverArea, numApps, adlApplications);
}


int ADL2_ApplicationProfiles_ConvertToCompact(ADL_CONTEXT_HANDLE context, CUSTOMISATIONS *pCustomisation, BINFILE *outCompact, int* outSize) {
    static ADL2_APPLICATIONPROFILES_CONVERTTOCOMPACT f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_ConvertToCompact");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, pCustomisation, outCompact, outSize);
}


int ADL2_ApplicationProfiles_GetCustomization(ADL_CONTEXT_HANDLE context, ADL_AP_DATABASE Databaseln, CUSTOMISATIONS *pCustomizationOut) {
    static ADL2_APPLICATIONPROFILES_GETCUSTOMIZATION f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_GetCustomization");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, Databaseln, pCustomizationOut);
}


int ADL2_ApplicationProfiles_Profile_Exist(ADL_CONTEXT_HANDLE context, const wchar_t* DriverArea, const wchar_t* ProfileName, BOOL* outExists) {
    static ADL2_APPLICATIONPROFILES_PROFILE_EXIST f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_Profile_Exist");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, DriverArea, ProfileName, outExists); 
}


int ADL2_ApplicationProfiles_ProfileApplicationX2_Assign(ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *Title, const wchar_t *DriverArea, const wchar_t *ProfileName) {
    static ADL2_APPLICATIONPROFILES_PROFILEAPPLICATIONX2_ASSIGN f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_ProfileApplicationX2_Assign");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, FileName, Path, Version, Title, DriverArea, ProfileName);
}


int ADL2_ApplicationProfiles_RemoveApplication(ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *DriverArea) {
    static ADL2_APPLICATIONPROFILES_REMOVEAPPLICATION f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_RemoveApplication");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context, FileName, Path, Version, DriverArea);
}


int ADL2_ApplicationProfiles_System_Reload(ADL_CONTEXT_HANDLE context) {
    static ADL2_APPLICATIONPROFILES_SYSTEM_RELOAD f = NULL;
    if(!f){
        f = get_function("ADL2_ApplicationProfiles_System_Reload");
    }
    if(!f){
        return INT_MIN;
    }
    return f(context);
}
