cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "/opt/carma/cmake/")

set(CMAKE_CXX_STANDARD 17)

project(udp-socket
    DESCRIPTION "Library include classes for UDP socket communication"
    HOMEPAGE_URL https://github.com/usdot-fhwa-stol/udp-socket
	VERSION 0.0.1
    LANGUAGES CXX
)


option(BUILD_TESTS "Build tests" OFF)
option(CREATE_DEB_PACKAGE "Create Debian Package" OFF)

add_library(${PROJECT_NAME} SHARED )

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

include(CommonSource)
if (BUILD_TESTS)
    include(Testing)
    include(CodeCoverage)
endif()
include(InstallingGeneral)
include(InstallingConfigs)

if (CREATE_DEB_PACKAGE)
    include(Packing)
endif()



