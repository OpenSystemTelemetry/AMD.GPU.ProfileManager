# Project Agents.md Guide

This Agents.md file provides comprehensive guidance for AI agents working with this codebase.

## Project Structure for AI Agent Navigation

- `/src`: Core C++ library source code that AI agents should analyze
  - `/include`: Header files defining the library's public interface
  - `/src`: Source files implementing the library's functionality
- `/src_app_cli`: A command-line interface for the core library
- `/src_app_gui`: A graphical user interface for the core library
- `/3rdparty`: Third-party libraries and dependencies. AI agents should not modify these directly.

## Coding Conventions for AI Agents

### General Conventions 

- Use C++26 for all new code
- Follow the existing code style in each file
- Use meaningful variable and function names
- Add comments for complex logic

### C++-Specific Guidelines

- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for memory management
- Prefer `const` and `constexpr` where appropriate
- Follow the rule of zero/three/five for resource management in classes
- Use modern C++ features when applicable

## Building and Testing

The project uses CMake for building. To build the project, an AI agent can use the following commands from the project root:

```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .
```

Currently, there are no testing frameworks integrated. If you add one, please update this section with instructions on how to run the tests.

## Pull Request Guidelines

When an AI agent helps create a PR, please ensure it:

1. Includes a clear description of the changes
2. References any related issues
3. Ensures the project builds successfully
4. Keeps PRs focused on a single concern

## A Note on CMake and Builds

- **Do not "fix" the CMake project:** The project's `CMakeLists.txt` files are correct. Do not alter them, even if your local environment suggests changes.
- **Ignore Phantom Header Errors:** You may see errors in your IDE about missing headers from `3rdparty` libraries. These are known phantom errors and do not affect the command-line build. The project will build successfully.

## Programmatic Checks

Before submitting changes, run the following:

```bash
# Configure the project
cmake ..

# Build the project
cmake --build .
```

All checks must pass before AI agent-generated code can be merged.
