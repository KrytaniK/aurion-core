cmake_minimum_required(VERSION 4.2)

# CMake option to disable/enable testing. Default enabled.
option(AURION_CORE_TEST "Enable testing the library" ON)

project(aurion-core
    VERSION 1.2.1
    DESCRIPTION "Core desktop-application systems for the Aurion Game Engine"
    LANGUAGES CXX
)

add_subdirectory(AurionCore)

if (AURION_CORE_TEST)
    enable_testing()
    add_subdirectory(Testing)
endif ()
