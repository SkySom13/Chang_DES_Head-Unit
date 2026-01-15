cmake_minimum_required(VERSION 3.16)

project(VehicleControlMock VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ════════════════════════════════════════════════════════════
# Deployment Environment
# ════════════════════════════════════════════════════════════
if(DEFINED ENV{DEPLOY_PREFIX})
    set(INSTALL_PREFIX $ENV{DEPLOY_PREFIX})
    message(STATUS "Using DEPLOY_PREFIX from environment: ${INSTALL_PREFIX}")
else()
    set(INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../../install_folder")
    message(STATUS "Using default DEPLOY_PREFIX: ${INSTALL_PREFIX}")
endif()

# RPATH settings
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${INSTALL_PREFIX}")
set(CMAKE_INSTALL_RPATH "${INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")

# ════════════════════════════════════════════════════════════
# Qt5
# ════════════════════════════════════════════════════════════
set(Qt5_DIR "/home/seame/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
set(CMAKE_PREFIX_PATH "/home/seame/Qt/5.15.2/gcc_64;${CMAKE_PREFIX_PATH}")

find_package(Qt5 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

# ════════════════════════════════════════════════════════════
# CommonAPI & vsomeip
# ════════════════════════════════════════════════════════════
find_package(CommonAPI 3.2 REQUIRED)
find_package(CommonAPI-SomeIP 3.2 REQUIRED)
find_package(vsomeip3 REQUIRED)

# CommonAPI generated code
if(NOT COMMONAPI_GEN_DIR)
    set(COMMONAPI_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../commonapi/generated")
endif()

message(STATUS "═══════════════════════════════════════════════════════")
message(STATUS "VehicleControlMock Configuration")
message(STATUS "  Qt5: /home/seame/Qt/5.15.2/gcc_64")
message(STATUS "  vsomeip: ${INSTALL_PREFIX}")
message(STATUS "  Generated code: ${COMMONAPI_GEN_DIR}")
message(STATUS "═══════════════════════════════════════════════════════")

include_directories(
    ${COMMONAPI_GEN_DIR}/core
    ${COMMONAPI_GEN_DIR}/someip
)

# ════════════════════════════════════════════════════════════
# Source Files
# ════════════════════════════════════════════════════════════
set(SOURCES
    src/main.cpp
    src/MockStubImpl.cpp
)

set(HEADERS
    src/MockStubImpl.h
)

# CommonAPI generated sources (Stub for service provider)
set(COMMONAPI_SOURCES
    ${COMMONAPI_GEN_DIR}/someip/v1/vehiclecontrol/VehicleControlSomeIPStubAdapter.cpp
    ${COMMONAPI_GEN_DIR}/someip/v1/vehiclecontrol/VehicleControlSomeIPDeployment.cpp
)

# ════════════════════════════════════════════════════════════
# Executable
# ════════════════════════════════════════════════════════════
add_executable(VehicleControlMock
    ${SOURCES}
    ${HEADERS}
    ${COMMONAPI_SOURCES}
)

target_link_libraries(VehicleControlMock
    Qt5::Core
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
)

# ════════════════════════════════════════════════════════════
# Install
# ════════════════════════════════════════════════════════════
install(TARGETS VehicleControlMock
    RUNTIME DESTINATION bin
)

message(STATUS "✓ VehicleControlMock configured successfully")
