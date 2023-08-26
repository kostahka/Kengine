# Windows export all symbols TODO: use __declspec(dllexport)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Kengine sources
set(KENGINE_SOURCES
    # cmake-format: sortable
    audio/audio.cxx
    audio/audio.hxx
    engine.cxx
    engine.hxx
    event/event.cxx
    event/event.hxx
    event/handle-user-event.hxx
    event/user-events.cxx
    file-last-modify-listener.cxx
    graphics/element-buffer.cxx
    graphics/raw-vertex-buffer.cxx
    graphics/render-manager.cxx
    graphics/render-manager.hxx
    graphics/shader.cxx
    graphics/vertex-array.cxx
    handle-file-modify.hxx
    helpers/hash.cxx
    imgui-impl/imgui_impl_opengl3.cpp
    imgui-impl/imgui_impl_opengl3.h
    imgui-impl/imgui_impl_opengl3_loader.h
    imgui-impl/imgui_impl_sdl3.cpp
    imgui-impl/imgui_impl_sdl3.h
    imgui-impl/imgui_impl_sdlrenderer3.cpp
    imgui-impl/imgui_impl_sdlrenderer3.h
    io/file-manager.cxx
    io/input.cxx
    log/log.cxx
    log/log.hxx
    opengl/opengl-debug.cxx
    opengl/opengl-debug.hxx
    opengl/opengl.cxx
    opengl/opengl.hxx
    render/animation.cxx
    render/b2GLDraw.cxx
    render/draw-primitives.cxx
    render/engine-resources.cxx
    render/picopng.hxx
    render/shader-program.cxx
    render/sprite.cxx
    render/texture.cxx
    render/vertex-array-object.cxx
    resources/resource.cxx
    resources/shader-resource.cxx
    string/string-id.cxx
    window/window.cxx
    window/window.hxx
)

set(KENGINE_HEADERS
    # cmake-format: sortable
    ../include/Kengine/audio/audio.hxx
    ../include/Kengine/engine.hxx
    ../include/Kengine/event/event.hxx
    ../include/Kengine/event/user-events.hxx
    ../include/Kengine/file-last-modify-listener.hxx
    ../include/Kengine/game.hxx
    ../include/Kengine/graphics/element-buffer.hxx
    ../include/Kengine/graphics/raw-vertex-buffer.hxx
    ../include/Kengine/graphics/render-manager.hxx
    ../include/Kengine/graphics/shader.hxx
    ../include/Kengine/graphics/vertex-array.hxx
    ../include/Kengine/graphics/vertex-buffer.hxx
    ../include/Kengine/helpers/hash.hxx
    ../include/Kengine/io/file-manager.hxx
    ../include/Kengine/io/input.hxx
    ../include/Kengine/log/log.hxx
    ../include/Kengine/render/animation.hxx
    ../include/Kengine/render/b2GLDraw.hxx
    ../include/Kengine/render/draw-primitives.hxx
    ../include/Kengine/render/engine-resources.hxx
    ../include/Kengine/render/shader-program.hxx
    ../include/Kengine/render/sprite.hxx
    ../include/Kengine/render/texture.hxx
    ../include/Kengine/render/vertex-array-object.hxx
    ../include/Kengine/resources/resource.hxx
    ../include/Kengine/resources/resource_types.hxx
    ../include/Kengine/resources/shader-resource.hxx
    ../include/Kengine/string/string-id.hxx
    ../include/Kengine/units/rect.hxx
    ../include/Kengine/units/transform.hxx
    ../include/Kengine/units/vector.hxx
    ../include/Kengine/units/vertex.hxx
    ../include/Kengine/window/window.hxx
)

set(KENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)
set(IMGUI_IMPL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui-impl)

# Kengine interfaces
add_library(kengine-private INTERFACE)
add_library(kengine-public INTERFACE)
add_library(kengine-interface INTERFACE)

# Link external libs
target_link_libraries(
    kengine-private
    INTERFACE SDL3::SDL3-static
    INTERFACE efsw
    INTERFACE glad
)
target_link_libraries(
    kengine-public
    INTERFACE imgui
    INTERFACE glm::glm
    INTERFACE box2d
    INTERFACE spdlog
)
if(ANDROID)
    target_link_libraries(
        kengine-public
        INTERFACE android
                  log
                  EGL
                  GLESv3
    )
endif()

# Compile definitions
target_compile_definitions(kengine-public INTERFACE IMGUI_IMPL_OPENGL_ES3)
if(WIN32)
    target_compile_definitions(
        kengine-private INTERFACE "-DE_DECLSPEC=__declspec(dllexport)"
    )
    target_compile_definitions(
        kengine-interface INTERFACE "-DE_DECLSPEC=__declspec(dllimport)"
    )
else()
    target_compile_definitions(kengine-public INTERFACE "-DE_DECLSPEC=")
endif()

# Include dirs
target_include_directories(kengine-public INTERFACE ${KENGINE_INCLUDE_DIR})
target_include_directories(kengine-private INTERFACE ${IMGUI_IMPL_INCLUDE_DIR})

# <<
# Develop engine target
# <<
if(ANDROID)
    add_library(kengine-dev SHARED ${KENGINE_SOURCES} ${KENGINE_HEADERS})
else()
    add_executable(kengine-dev ${KENGINE_SOURCES} ${KENGINE_HEADERS})
endif()

target_link_libraries(
    kengine-dev
    PUBLIC kengine-public
    PRIVATE kengine-private
    INTERFACE kengine-interface
)

target_compile_definitions(
    kengine-dev
    PUBLIC ENGINE_DEV
    PUBLIC ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME}"
)

# Enable exports for game library to link
set_target_properties(kengine-dev PROPERTIES ENABLE_EXPORTS ON)

# <<
# Shared engine lib
# <<
add_library(kengine-shared SHARED ${KENGINE_SOURCES} ${KENGINE_HEADERS})

target_link_libraries(
    kengine-shared
    PUBLIC kengine-public
    PRIVATE kengine-private
    INTERFACE kengine-interface
)

# <<
# Static engine lib
# <<
add_library(kengine-static STATIC ${KENGINE_SOURCES} ${KENGINE_HEADERS})

target_link_libraries(
    kengine-static
    PUBLIC kengine-public
    PRIVATE kengine-private
    INTERFACE kengine-interface
)

# PIC for static lib
set_property(TARGET kengine-static PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(Kengine::kengine-static ALIAS kengine-static)
add_library(Kengine::kengine-shared ALIAS kengine-shared)
add_executable(Kengine::kengine-dev ALIAS kengine-dev)

install(
    TARGETS kengine-static kengine-shared kengine-dev
    DESTINATION ${CMAKE_BINARY_DIR}/dist
    COMPONENT Kengine
)
