# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../vendor/raylib ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/raylib)
endif()

# raylib-tiled-example
add_executable(raylib-tiled-example raylib-tiled-example.c)
target_link_libraries(raylib-tiled-example PUBLIC raylib raylib-tiled)

# Copy the resources
file(GLOB resources resources/*)
set(example_resources)
list(APPEND example_resources ${resources})
file(COPY ${example_resources} DESTINATION "resources/")