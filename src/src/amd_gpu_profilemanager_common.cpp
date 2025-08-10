// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//

// AMD ADL SDK
#include <adl_sdk_extra.h>

// AMD GPU Profile Manager
#include "amd_gpu_profilemanager_common.h"



//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    namespace Common {
        static void* __stdcall adl_malloc ( int iSize ) {
            void* lpBuffer = malloc ( iSize );
            return lpBuffer;
        }

        ADL_CONTEXT_HANDLE Initialize() {
            ADL_CONTEXT_HANDLE result{};

            if (ADL2_Main_Control_Create(adl_malloc, 1, &result) != ADL_OK)
            {
                result = nullptr;
            }
            return result;
        }

        bool Deinitialize(ADL_CONTEXT_HANDLE handle) {
            if (!handle) {
                return false;
            }
            return ADL2_Main_Control_Destroy(handle) == ADL_OK;
        }
    }
}
