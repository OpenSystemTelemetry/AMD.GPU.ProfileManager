# AMD GPU Profile Manager

[![build](https://github.com/OpenSystemTelemetry/AMD.GPU.ProfileManager/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/OpenSystemTelemetry/AMD.GPU.ProfileManager/actions/workflows/build.yml)

AMD GPU Profile Manager is a utility for managing AMD GPU application profiles. It provides a interface to view, create, and manage GPU settings on a per-application basis.

## Download

You can download the latest release from the [GitHub Releases](https://github.com/OpenSystemTelemetry/AMD.GPU.ProfileManager/releases/latest) page.

## Features

- **View Profile Databases**: Inspect application profiles from System, User, and OEM databases.
- **Search and Filter**: Find applications in the database.
- **FSR Whitelisting**: whitelist FSR for any application.

## Building the Project

To build the project, you will need CMake installed. Follow these steps from the root of the project directory:

```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .
```

After a successful build, you can find the `ost_amd_gpu_profilemanager_gui` executable in the `build/bin` directory.
