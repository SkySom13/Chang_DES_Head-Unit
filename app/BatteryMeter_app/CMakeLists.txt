cmake_minimum_required(VERSION 3.16)

project(BatteryMeter_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ════════════════════════════════════════════════════════════
# Qt Quick QML Build with MOCK VehicleControl (No vsomeip)
# Using Qt 5.15.2 from /home/seame/Qt/5.15.2
# ════════════════════════════════════════════════════════════
message(STATUS "══════════════════════════════════════════════════════")
message(STATUS "Building BatteryMeter_app - MOCK MODE")
message(STATUS "Qt from: /home/seame/Qt/5.15.2/gcc_64")
message(STATUS "No vsomeip dependencies (using mock data)")
message(STATUS "══════════════════════════════════════════════════════")

# Set Qt5 paths
set(Qt5_DIR "/home/seame/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
set(CMAKE_PREFIX_PATH "/home/seame/Qt/5.15.2/gcc_64;${CMAKE_PREFIX_PATH}")

# Find Qt5 packages (NO CommonAPI or vsomeip)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick Network)
set(QT_VERSION_MAJOR 5)

# Project sources (mock version only)
set(PROJECT_SOURCES
    src/main.cpp
    src/vehiclecontrolclient.cpp
    src/vehiclecontrolclient.h
    resources/BatteryMeter.qrc
)

# Create executable
add_executable(BatteryMeter_app
    ${PROJECT_SOURCES}
)

# Link only Qt libraries (NO CommonAPI, NO vsomeip)
target_link_libraries(BatteryMeter_app
    PRIVATE 
        Qt5::Core 
        Qt5::Gui 
        Qt5::Qml 
        Qt5::Quick 
        Qt5::Network
)

# Set target properties
set_target_properties(BatteryMeter_app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.seame.BatteryMeter_app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

message(STATUS "✓ BatteryMeter_app configured (MOCK mode - no vsomeip)")
message(STATUS "══════════════════════════════════════════════════════")
