# C++ Standard 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Source files & Modules
file(GLOB_RECURSE MACROS CONFIGURE_DEPENDS "macros/*.h")
file(GLOB_RECURSE MOD CONFIGURE_DEPENDS "modules/*.ixx")
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")

# Project is a shared library
add_library(aurion-core SHARED ${SRC} ${MACROS})

target_sources(aurion-core
        PUBLIC
            FILE_SET CXX_MODULES FILES ${MOD}
)

target_include_directories(aurion-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/macros")

# Build targets
install(TARGETS aurion-core
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${MOD} DESTINATION modules)
install(DIRECTORY macros/ DESTINATION include)

# Global Definitions
target_compile_definitions(aurion-core PRIVATE AURION_CORE_EXPORTS)

# Platform-specific Definitions
if (WIN32)
    target_compile_definitions(aurion-core PRIVATE AURION_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(aurion-core PRIVATE AURION_PLATFORM_LINUX)
endif()

# ----- Project Dependencies

# GLFW
include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

target_link_libraries(aurion-core PRIVATE glfw)