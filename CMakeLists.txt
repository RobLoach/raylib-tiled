cmake_minimum_required(VERSION 3.11)
project (raylib-tiled
    VERSION 0.0.1
    DESCRIPTION "raylib-tiled"
    HOMEPAGE_URL "https://github.com/robloach/raylib-tiled"
    LANGUAGES C)

# Include Directory
add_subdirectory(include)

# Examples
option(BUILD_RAYLIB_TILED_EXAMPLE "Example" ON)
if(BUILD_RAYLIB_TILED_EXAMPLE)
    add_subdirectory(example)
endif()

# Testing
# TODO: Add testing.
# include(CTest)
# enable_testing()
# if(BUILD_TESTING)
#     set(CTEST_CUSTOM_TESTS_IGNORE
#         pkg-config--static
#     )
#     add_subdirectory(test)
# endif()
