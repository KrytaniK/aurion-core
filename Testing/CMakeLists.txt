# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Aurion Core uses GoogleTest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# On Windows machines, prevent overriding the parent project's compiler/linker settings
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

FetchContent_MakeAvailable(googletest)

# Test Sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

# Add test project and link library and testing framework
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests
    PRIVATE aurion-core-shared GTest::gtest_main
)

# Set to auto-discover Testing cases
include(GoogleTest)
gtest_discover_tests(unit_tests)