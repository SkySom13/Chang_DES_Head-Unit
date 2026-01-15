# AGENTS.md - Development Guide for DES Head-Unit

This file provides essential information for AI agents working on the DES Head-Unit automotive infotainment system.

## Build System & Commands

### Primary Build System
- **CMake** (version 3.16+) with C++17 standard
- **Root CMakeLists.txt** provides modular build options for multi-process architecture
- Default build type: Debug (can be changed with `-DCMAKE_BUILD_TYPE=Release`)

### Build Commands
```bash
# Build all enabled apps from root
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# Build specific app (example: MediaApp)
cd app/MediaApp
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# Cross-compile for Raspberry Pi (via Yocto)
# See DEPLOYMENT_GUIDE.md for full Yocto setup
```

### Testing Commands
```bash
# PDC Integration Test (builds and runs multi-app scenario)
./app/run_pdc_test.sh build    # Build only
./app/run_pdc_test.sh run      # Run only (assumes built)
./app/run_pdc_test.sh all      # Build and run (default)

# Manual testing for individual apps
# Each app can be run independently from its build directory

# Run single app (example: MediaApp)
cd app/MediaApp
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
./build/MediaApp
```

### Linting & Code Quality
- No automated linting configured in CI/CD
- Manual code review required for C++/QML style compliance
- Use Qt Creator for IDE-level syntax checking

## Code Style Guidelines

### C++ Style (Qt-based)
- **Naming Conventions**:
  - Classes: PascalCase (e.g., `MediaManager`, `VehicleControlStubImpl`)
  - Methods/Functions: camelCase (e.g., `playMedia()`, `setGearState()`)
  - Variables: camelCase (e.g., `currentTrack`, `isPlaying`)
  - Constants: UPPER_SNAKE_CASE (e.g., `MAX_VOLUME_LEVEL`)
  - Private members: m_ prefix (e.g., `m_mediaManager`)

- **Import Order**:
  ```cpp
  // 1. Standard library
  #include <iostream>
  #include <memory>
  
  // 2. Qt includes
  #include <QGuiApplication>
  #include <QQmlApplicationEngine>
  #include <QMediaPlayer>
  
  // 3. Third-party (CommonAPI, vsomeip, Boost)
  #include <CommonAPI/CommonAPI.hpp>
  #include <vsomeip/vsomeip.hpp>
  #include <boost/system.hpp>
  
  // 4. Local includes
  #include "mediamanager.h"
  #include "MediaControlStubImpl.h"
  ```

- **Qt Signal-Slot Patterns**:
  - Use Qt5 connect syntax: `connect(sender, &Sender::signal, receiver, &Receiver::slot)`
  - Prefer signals/slots over direct function calls for inter-component communication
  - Use `Q_EMIT` macro for signal emission

- **Error Handling**:
  - Use Qt's logging framework: `qDebug()`, `qWarning()`, `qCritical()`
  - CommonAPI runtime exceptions should be caught with try-catch blocks
  - vSOME/IP connection errors handled via callback functions

- **International Comments**:
  - Korean comments mixed with English (international development team)
  - Decorative separators: `═════════════` for major sections
  - Function-level documentation expected for public APIs

### QML Style
- **Component Structure**:
  ```qml
  import QtQuick 2.12
  import QtQuick.Controls 2.12
  
  Rectangle {
      id: root
      
      // Properties
      property color themeColor: "#2196F3"
      property bool isActive: false
      
      // Signal handlers
      onIsActiveChanged: {
          // Handle state change
      }
      
      // Child elements
      Text {
          // UI element
      }
  }
  ```

- **Naming**: camelCase for IDs and properties, PascalCase for custom components

## Architecture Patterns

### Multi-Process Architecture
- Each Qt app runs as separate process (MediaApp, GearApp, AmbientApp, etc.)
- **HU_MainApp** acts as Wayland compositor for other apps
- **vSOME/IP** provides inter-process communication
- **CommonAPI** abstracts service interfaces

### Service-Oriented Architecture
- **FIDL Definitions**: `commonapi/fidl/` directory contains interface definitions
- **Generated Code**: `commonapi/generated/` contains C++ stub/proxy code
- **Service Implementation**: Each app provides/consumes services via CommonAPI

### Environment Variables
Critical for runtime configuration:
```bash
# vSOME/IP configuration
export VSOMEIP_APPLICATION_NAME=MediaApp
export VSOMEIP_CONFIGURATION=/path/to/vsomeip.json

# CommonAPI configuration
export COMMONAPI_CONFIG=/path/to/commonapi.ini

# Qt platform (Wayland for embedded, xcb for desktop testing)
export QT_QPA_PLATFORM=wayland
export QT_WAYLAND_DISABLE_WINDOWDECORATION=1
```

## Dependencies & Libraries

### Core Dependencies
- **Qt5**: Core, Multimedia, Quick, Qml components
- **CommonAPI 3.2.4+**: IPC abstraction layer
- **vSOME/IP 3.5.8+**: SOME/IP protocol implementation
- **Boost 1.74+**: System, Thread, Filesystem components

### Build Dependencies
- **CMake 3.16+**: Build system
- **Yocto**: Embedded Linux cross-compilation
- **Wayland/Weston**: Display server for embedded deployment

## Development Workflow

### Local Development (x86_64)
1. Install Qt5 development packages
2. Build with CMake using default settings
3. Run with `QT_QPA_PLATFORM=xcb` for desktop testing
4. Use `QT_LOGGING_RULES="*.debug=true"` for verbose logging

### Target Deployment (Raspberry Pi/Jetson)
1. Cross-compile via Yocto or use pre-built binaries
2. Deploy using scripts in `app/config/` directory
3. Configure environment variables for target hardware
4. Use Wayland compositor for display management

### Testing Strategy
- **Integration Testing**: Real hardware testing with `run_pdc_test.sh`
- **Manual Testing**: Individual app testing with mock services
- **Network Testing**: vSOME/IP communication validation
- **No Unit Testing**: Project lacks automated unit testing framework

## Common Pitfalls & Solutions

### vSOME/IP Issues
- Ensure `VSOMEIP_APPLICATION_NAME` matches JSON configuration
- Check network connectivity between ECUs (192.168.1.x subnet)
- Verify routing manager is running on service provider ECU

### Qt/QML Issues
- Use correct `QT_QPA_PLATFORM` (wayland for embedded, xcb for desktop)
- Ensure QML import paths are correctly set
- Check Wayland socket availability (`/run/user/$UID/wayland-1`)

### Build Issues
- Set `DEPLOY_PREFIX` environment variable for custom install locations
- Ensure CommonAPI generated code is available before building
- Use proper RPATH settings for library discovery

## File Organization
- **C++ Files**: `src/` directory within each app
- **QML Files**: `qml/` directory or alongside C++ files
- **Headers**: `.h` files in `src/` directory
- **Config**: `config/` directory for JSON/INI files
- **Resources**: `images/`, `assets/` directories

## Security Considerations
- No hardcoded credentials in source code
- Environment variables used for configuration
- Network communication limited to local subnet
- Input validation required for external data sources

## File Organization
- **C++ Files**: `src/` directory within each app
- **QML Files**: `qml/` directory or alongside C++ files
- **Headers**: `.h` files in `src/` directory
- **Config**: `config/` directory for JSON/INI files
- **Resources**: `images/`, `assets/` directories

## Security Considerations
- No hardcoded credentials in source code
- Environment variables used for configuration
- Network communication limited to local subnet
- Input validation required for external data sources

## File Organization
- **C++ Files**: `src/` directory within each app
- **QML Files**: `qml/` directory or alongside C++ files
- **Headers**: `.h` files in `src/` directory
- **Config**: `config/` directory for JSON/INI files
- **Resources**: `images/`, `assets/` directories

## Security Considerations
- No hardcoded credentials in source code
- Environment variables used for configuration
- Network communication limited to local subnet
- Input validation required for external data sources

## Performance Guidelines
- Use Qt's signal-slot mechanism for loose coupling
- Minimize vSOME/IP message frequency for real-time requirements
- Consider memory constraints on embedded targets
- Profile with Qt's built-in performance tools