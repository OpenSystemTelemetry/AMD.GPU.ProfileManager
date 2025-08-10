// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

//
// Includes
//

// Windows
#if defined(_WIN32)
#include <windows.h>
#endif

// AMD Display Library
#include <adl_sdk.h>
#include <adl_sdk_extra_structures.h>



//
// Functions/Main
//

typedef int (*ADL2_MAIN_CONTROL_CREATE )(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADL_CONTEXT_HANDLE * context);
        int   ADL2_Main_Control_Create  (ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADL_CONTEXT_HANDLE * context);

typedef int (*ADL2_MAIN_CONTROL_DESTROY)(ADL_CONTEXT_HANDLE context);
        int   ADL2_Main_Control_Destroy	(ADL_CONTEXT_HANDLE context);	



//
// Functions/ApplicationProfiles
//

typedef int (*ADL2_APPLICATIONPROFILES_APPLICATIONS_GET           )(ADL_CONTEXT_HANDLE context, const wchar_t* driverArea, int* numApps, ADLApplicationRecord** adlApplications);
        int   ADL2_ApplicationProfiles_Applications_Get            (ADL_CONTEXT_HANDLE context, const wchar_t* driverArea, int* numApps, ADLApplicationRecord** adlApplications);

typedef int (*ADL2_APPLICATIONPROFILES_CONVERTTOCOMPACT           )(ADL_CONTEXT_HANDLE context, CUSTOMISATIONS *pCustomisation, BINFILE *outCompact, int* outSize);
        int   ADL2_ApplicationProfiles_ConvertToCompact            (ADL_CONTEXT_HANDLE context, CUSTOMISATIONS *pCustomisation, BINFILE *outCompact, int* outSize);

typedef int (*ADL2_APPLICATIONPROFILES_GETCUSTOMIZATION           )(ADL_CONTEXT_HANDLE context, ADL_AP_DATABASE Databaseln, CUSTOMISATIONS *pCustomizationOut);
        int   ADL2_ApplicationProfiles_GetCustomization            (ADL_CONTEXT_HANDLE context, ADL_AP_DATABASE Databaseln, CUSTOMISATIONS *pCustomizationOut);

typedef int (*ADL_APPLICATIONPROFILES_GETCUSTOMIZATION           )(ADL_AP_DATABASE Databaseln, CUSTOMISATIONS *pCustomizationOut);
        int   ADL_ApplicationProfiles_GetCustomization            (ADL_AP_DATABASE Databaseln, CUSTOMISATIONS *pCustomizationOut);

typedef int (*ADL2_APPLICATIONPROFILES_PROFILE_EXIST              )(ADL_CONTEXT_HANDLE context, const wchar_t* DriverArea, const wchar_t* ProfileName, BOOL* outExists);
        int   ADL2_ApplicationProfiles_Profile_Exist               (ADL_CONTEXT_HANDLE context, const wchar_t* DriverArea, const wchar_t* ProfileName, BOOL* outExists);

typedef int (*ADL2_APPLICATIONPROFILES_PROFILEAPPLICATIONX2_ASSIGN)(ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *Title, const wchar_t *DriverArea, const wchar_t *ProfileName);
        int   ADL2_ApplicationProfiles_ProfileApplicationX2_Assign (ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *Title, const wchar_t *DriverArea, const wchar_t *ProfileName);

typedef int (*ADL2_APPLICATIONPROFILES_REMOVEAPPLICATION          )(ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *DriverArea);
        int   ADL2_ApplicationProfiles_RemoveApplication           (ADL_CONTEXT_HANDLE context, const wchar_t *FileName, const wchar_t *Path, const wchar_t *Version, const wchar_t *DriverArea);


typedef int (*ADL2_APPLICATIONPROFILES_SYSTEM_RELOAD              )(ADL_CONTEXT_HANDLE context);
        int   ADL2_ApplicationProfiles_System_Reload               (ADL_CONTEXT_HANDLE context);

#if defined(__cplusplus)
}
#endif
