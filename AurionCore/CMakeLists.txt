# ============================================================================
#  AurionCore — project build script
#
#  Sections:
#    (1) PROJECT SETUP     options, toolchain-level switches
#    (2) DEPENDENCIES      third-party requirements (find_package/FetchContent)
#    (3) TARGETS           sources and the static/shared library flavours
#    (4) EXPORT            install rules and package config generation
#
#  Can build both static/shared libraries, or build them individually.
#  Consumers link one of:  Aurion::Core (default) | Aurion::Core::Shared
#                                                 | Aurion::Core::Static
# ============================================================================

# ----------------------------------------------------------------------------
#  PROJECT SETUP
# ----------------------------------------------------------------------------

# Use default library path for install
include(GNUInstallDirs)
# For retrieving project dependencies
include(FetchContent)
# Feature for generating the .cmake file (for find_package)
include(CMakePackageConfigHelpers)

option(AURION_CORE_SHARED "Build as a shared library" ON)
option(AURION_CORE_STATIC "Build as a shared library" ON)
option(AURION_CORE_INSTALL "Emit install and export rules" ON)

if (NOT AURION_CORE_SHARED AND NOT AURION_CORE_STATIC)
    message(FATAL_ERROR "Enable at least one of AURION_CORE_SHARED / AURION_CORE_STATIC.")
endif()

# ----------------------------------------------------------------------------
#  DEPENDENCIES
#   Aurion Core is nearly self-contained. Third-party requirements are
#   declared here, with a preference to find_package.
# ----------------------------------------------------------------------------

# Build GLFW from source to make linking platform-dependent.
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        SYSTEM
        EXCLUDE_FROM_ALL
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# ----------------------------------------------------------------------------
#  TARGETS
# ----------------------------------------------------------------------------

# Source files & Modules
file(GLOB_RECURSE AURION_CORE_HEADERS CONFIGURE_DEPENDS "macros/*.h")
file(GLOB_RECURSE AURION_CORE_MODULE_INTERFACES CONFIGURE_DEPENDS "modules/*.ixx")
file(GLOB_RECURSE AURION_CORE_MODULE_SOURCES    CONFIGURE_DEPENDS "src/*.cpp")

# ----- Shared usage requirements: -------------------------------------------
#  Everything common to both shared/static library configurations lives here
# ----------------------------------------------------------------------------

add_library(aurion-core-common INTERFACE)

# Specify C++ standard 23 (latest)
TARGET_COMPILE_FEATURES(aurion-core-common INTERFACE cxx_std_23)

# Project Dependencies
target_link_libraries(aurion-core-common INTERFACE glfw)

# The common interface is strictly header-only
target_sources(aurion-core-common INTERFACE
    FILE_SET core_headers
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/macros"
    FILES ${AURION_CORE_HEADERS}
)

# Platform-Specific Preprocessor Definitions
if (WIN32)
    target_compile_definitions(aurion-core-common INTERFACE AURION_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(aurion-core-common INTERFACE AURION_PLATFORM_LINUX)
endif()

# ----- Shared Library Configuration: ----------------------------------------
# ----------------------------------------------------------------------------

if (AURION_CORE_SHARED)
    add_library(aurion-core-shared SHARED)
    target_link_libraries(aurion-core-shared PUBLIC aurion-core-common)

    set_target_properties(aurion-core-shared PROPERTIES
            OUTPUT_NAME "AurionCore-Shared"
            DEBUG_POSTFIX "d"
    )

    # Enable the API macro for shared build
    target_compile_definitions(aurion-core-shared PRIVATE AURION_CORE_SHARED_COMPILE)

    target_sources(aurion-core-shared
        PUBLIC
            FILE_SET core_modules
            TYPE CXX_MODULES
            BASE_DIRS "modules"
            FILES ${AURION_CORE_MODULE_INTERFACES}
        PRIVATE
            ${AURION_CORE_MODULE_SOURCES}
    )

    # Build Config Alias
    add_library(Aurion::Core::Shared ALIAS aurion-core-shared)
endif ()

# ----- Static Library Configuration: ----------------------------------------
# ----------------------------------------------------------------------------

if (AURION_CORE_STATIC)
    add_library(aurion-core-static STATIC)
    target_link_libraries(aurion-core-static PUBLIC aurion-core-common)

    set_target_properties(aurion-core-static PROPERTIES
            OUTPUT_NAME "AurionCore-Static"
            DEBUG_POSTFIX "d"
    )

    # Disable the API macro for static build
    target_compile_definitions(aurion-core-static PRIVATE AURION_CORE_STATIC_COMPILE)

    target_sources(aurion-core-static
            PUBLIC
            FILE_SET core_modules
            TYPE CXX_MODULES
            BASE_DIRS "modules"
            FILES ${AURION_CORE_MODULE_INTERFACES}
            PRIVATE
            ${AURION_CORE_MODULE_SOURCES}
    )

    # Build Config Alias
    add_library(Aurion::Core::Static ALIAS aurion-core-static)
endif ()

# ----- Convenience Target & Alias -------------------------------------------
# ----------------------------------------------------------------------------

# Add a combined target for building both the shared and static libraries
add_custom_target(aurion-core)
add_dependencies(aurion-core aurion-core-shared aurion-core-static)

# Default alias: Prefer shared when available, static otherwise
if (AURION_CORE_SHARED)
    add_library(Aurion::Core ALIAS aurion-core-shared)
else()
    add_library(Aurion::Core ALIAS aurion-core-static)
endif ()

# ----------------------------------------------------------------------------
#  EXPORT
# ----------------------------------------------------------------------------

if (AURION_CORE_INSTALL)
    # Append install targets based on configured cmake build
    set(AURION_CORE_TARGETS "")
    if (AURION_CORE_SHARED)
        list(APPEND AURION_CORE_TARGETS aurion-core-shared)
    endif()
    if (AURION_CORE_STATIC)
        list(APPEND AURION_CORE_TARGETS aurion-core-static)
    endif()

    # Write export targets to relevant folders
    install(TARGETS ${AURION_CORE_TARGETS} aurion-core-common
        EXPORT AurionCoreTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT AurionCore_Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT AurionCore_Runtime
        NAMELINK_COMPONENT AurionCore_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT AurionCore_Development
        FILE_SET core_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT   AurionCore_Development
        FILE_SET core_modules
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Aurion/modules
        COMPONENT   AurionCore_Development
    )

    # Write module-consumption scripts into the modules directory.
    # Each export set gets its own unique directory.
    install(EXPORT AurionCoreTargets
            NAMESPACE             Aurion::
            DESTINATION           ${CMAKE_INSTALL_LIBDIR}/cmake/AurionCore
            CXX_MODULES_DIRECTORY cxx-modules
            FILE                  AurionCoreTargets.cmake
            COMPONENT             AurionCore_Development
    )

    # CPack configuration
    set(CPACK_PACKAGE_NAME "AurionCore")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE")
    set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
    set(CPACK_PACKAGE_VERSION_TWEAK "${${PROJECT_NAME}_VERSION_TWEAK}")
    set(CPACK_GENERATOR "ZIP")

    # Let the folder path include the library version
    file(
        TO_NATIVE_PATH
        "${PROJECT_NAME}/${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}"
        suggested_path
    )

    # Update CPack path and filename
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${suggested_path}")

    # Helpers for generating an installer
    include(InstallRequiredSystemLibraries)
    include(CPack)
endif()