add_library(raylib-tiled INTERFACE)

# Include Directory
target_include_directories(raylib-tiled INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)

# Set the header files as install files.
install(FILES
    raylib-tiled.h
    cute_tiled.h
    DESTINATION include
)
