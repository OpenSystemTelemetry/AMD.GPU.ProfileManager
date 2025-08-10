// Copyright 2025, Open System Telemetry contributors
// SPDX-License-Identifier: MIT

#pragma once

//
// Includes
//



//
// Structs
//

namespace OST::AMD::GPU::ProfileManager {
    namespace Common {
        ADL_CONTEXT_HANDLE Initialize();
        bool Deinitialize(ADL_CONTEXT_HANDLE handle);
    }
}
