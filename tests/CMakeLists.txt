include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Prevent overriding parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Create test executable
add_executable(unit_tests
        vector2d_test.cpp
        transform_test.cpp
        collider_test.cpp
        # Add more test files here
)

target_link_libraries(unit_tests
        PRIVATE
        GameFramework
        GTest::gtest_main
        GTest::gmock_main
)

# Enable CMake's test discovery
include(GoogleTest)
gtest_discover_tests(unit_tests)