cmake_minimum_required(VERSION 3.15.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_UNITY_BUILD ON)

project(Kengine VERSION 0.0.0)

# Options for Kengine.
option(KENGINE_EDITOR "Build kengine editor" ON)
option(KENGINE_DEMO_GAME "Build demo game" ON)
option(KENGINE_IMGUI "Integration with imgui" ON)

if(KENGINE_EDITOR OR KENGINE_DEMO_GAME)
    set(KENGINE_IMGUI
        ON
        CACHE BOOL "Integration with imgui" FORCE
    )
endif()

# SDL3. For android provide sdl3.dir in local.properties in android-project
if(SDL3_SRC_DIR AND ANDROID)
    set(SDL_STATIC ON)
    set(SDL_STATIC_PIC ON)
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

target_compile_definitions(box2d PRIVATE B2_SHARED)
target_compile_definitions(box2d PRIVATE box2d_EXPORTS)

# EFSW - cross-platform file watcher and notifier
add_subdirectory(external/efsw)

# GLM - OpenGL Mathematics. C++ mathematics library for graphics software based
# on the OpenGL Shading Language (GLSL) specifications
option(GLM_TEST_ENABLE OFF)
add_subdirectory(external/glm)

# IMGUI
add_subdirectory(external/imgui-src)

# GLAD - for loading OpenGL functions
add_subdirectory(external/glad)

# STB(image) - for loading images
add_subdirectory(external/stb)

# SPDLOG
option(SPDLOG_BUILD_PIC ON)
option(SPDLOG_NO_EXCEPTIONS ON)
option(SPDLOG_NO_THREAD_ID ON)
option(SPDLOG_NO_TLS ON)
option(SPDLOG_NO_ATOMIC_LEVELS ON)
add_subdirectory(external/spdlog)
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(spdlog PRIVATE SPDLOG_SHARED_LIB)
target_compile_definitions(spdlog PRIVATE spdlog_EXPORTS)
target_compile_definitions(
    spdlog
    PRIVATE FMT_EXPORT
    PRIVATE FMT_SHARED
)
if(MSVC)

    target_compile_options(
        spdlog
        PUBLIC
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<COMPILE_LANGUAGE:CUDA>>>:/wd4251
            /wd4275>
    )

endif()

# ENTT
add_subdirectory(external/entt)

# SDL_ttf
find_package(SDL3_ttf REQUIRED)

# Kengine source
add_subdirectory(src)

# Demo game
if(KENGINE_DEMO_GAME)
    message(STATUS "Configuring demo game for Kengine.")
    add_subdirectory(demo-game)
endif()

# Kengine editor
if(KENGINE_EDITOR)
    message(STATUS "Configuring kengine editor")
    add_subdirectory(editor)
endif()
