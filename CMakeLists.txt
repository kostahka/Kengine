cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Kengine VERSION 0.0.0)

# Options for Kengine. You develop your game with hot-reload by setting
# ENGINE_DEV=On and setting ENGINE_GAME_LIB_NAME to name of your game target. In
# this case you need to build your game as shared lib and engine will build as
# executable.
set(KENGINE_DEV
    OFF
    CACHE BOOL "engine load game dll with hot reload"
)
set(KENGINE_GAME_LIB_NAME
    ""
    CACHE STRING "game lib name to load"
)

# SDL3. For android provide sdl3.dir in local.properties in android-project
if(SDL3_SRC_DIR AND ANDROID)
    option(SDL_STATIC ON)
    option(SDL_STATIC_PIC ON)
    add_subdirectory(
        # Specifies the directory of the CMakeLists.txt file.
        ${SDL3_SRC_DIR}
        # Specifies the directory for the build outputs.
        ${CMAKE_CURRENT_BINARY_DIR}/SDL_${ANDROID_ABI}
    )
else()
    find_package(SDL3 REQUIRED)
endif()

# Build static external libs
option(BUILD_SHARED_LIBS OFF)

# Box2D - 2D physics engine for games
option(BOX2D_BUILD_UNIT_TESTS OFF)
option(BOX2D_BUILD_TESTBED OFF)
option(BOX2D_BUILD_DOCS OFF)
option(BOX2D_USER_SETTINGS OFF)

add_subdirectory(external/box2d)
set_property(TARGET box2d PROPERTY POSITION_INDEPENDENT_CODE ON)

# EFSW - cross-platform file watcher and notifier
add_subdirectory(external/efsw)

# GLM - OpenGL Mathematics. C++ mathematics library for graphics software based
# on the OpenGL Shading Language (GLSL) specifications
add_subdirectory(external/glm)

# IMGUI
add_subdirectory(external/imgui-src)

# GLAD - for loading OpenGL functions
add_subdirectory(external/glad)

# Kengine source
add_subdirectory(src)
