# Windows export all symbols TODO: use __declspec(dllexport)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Kengine sources
set(KENGINE_SOURCES
    # cmake-format: sortable
    ${CMAKE_CURRENT_LIST_DIR}/audio/audio.cxx
    ${CMAKE_CURRENT_LIST_DIR}/audio/audio.hxx
    ${CMAKE_CURRENT_LIST_DIR}/configuration/configuration-file.cxx
    ${CMAKE_CURRENT_LIST_DIR}/configuration/configuration.cxx
    ${CMAKE_CURRENT_LIST_DIR}/engine.cxx
    ${CMAKE_CURRENT_LIST_DIR}/engine.hxx
    ${CMAKE_CURRENT_LIST_DIR}/event/event.cxx
    ${CMAKE_CURRENT_LIST_DIR}/event/event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/event/handle-user-event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/event/user-events.cxx
    ${CMAKE_CURRENT_LIST_DIR}/file-last-modify-listener.cxx
    ${CMAKE_CURRENT_LIST_DIR}/game-objects/component.cxx
    ${CMAKE_CURRENT_LIST_DIR}/game-objects/game-object.cxx
    ${CMAKE_CURRENT_LIST_DIR}/game-objects/physics-component.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/camera.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/element-buffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/framebuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/raw-vertex-buffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/render-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/render-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/renderbuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/shader-utils/shader-type.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/shader.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/texture.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/uniformbuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/graphics/vertex-array.cxx
    ${CMAKE_CURRENT_LIST_DIR}/handle-file-modify.hxx
    ${CMAKE_CURRENT_LIST_DIR}/helpers/hash.cxx
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_opengl3_loader.h
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_sdl3.h
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_sdlrenderer3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui-impl/imgui_impl_sdlrenderer3.h
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cxx
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.hxx
    ${CMAKE_CURRENT_LIST_DIR}/io/file-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/io/input.cxx
    ${CMAKE_CURRENT_LIST_DIR}/lib/lib.cxx
    ${CMAKE_CURRENT_LIST_DIR}/log/log.cxx
    ${CMAKE_CURRENT_LIST_DIR}/log/log.hxx
    ${CMAKE_CURRENT_LIST_DIR}/opengl/opengl-debug.cxx
    ${CMAKE_CURRENT_LIST_DIR}/opengl/opengl-debug.hxx
    ${CMAKE_CURRENT_LIST_DIR}/opengl/opengl.cxx
    ${CMAKE_CURRENT_LIST_DIR}/opengl/opengl.hxx
    ${CMAKE_CURRENT_LIST_DIR}/render/animation.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/b2GLDraw.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/draw-primitives.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/engine-resources.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/shader-program.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/sprite.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/texture.cxx
    ${CMAKE_CURRENT_LIST_DIR}/render/vertex-array-object.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/engine-resources.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/engine-resources.hxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/framebuffer-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/picopng.hxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/renderbuffer-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/resource-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/shader-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/resources/texture-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/string/string-id.cxx
    ${CMAKE_CURRENT_LIST_DIR}/window/display.cxx
    ${CMAKE_CURRENT_LIST_DIR}/window/display.hxx
    ${CMAKE_CURRENT_LIST_DIR}/window/window.cxx
    ${CMAKE_CURRENT_LIST_DIR}/window/window.hxx
)

set(KENGINE_HEADERS
    # cmake-format: sortable
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/audio/audio.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/configuration/configuration-file.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/configuration/configuration.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/engine.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/event/event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/event/user-events.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/file-last-modify-listener.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game-objects/component-types.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game-objects/component.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game-objects/game-object.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game-objects/physics-component.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/camera.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/element-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/framebuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/raw-vertex-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/render-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/renderbuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/shader-utils/shader-type.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/shader.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/texture-common.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/texture.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/uniformbuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/vertex-array.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/vertex-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/helpers/hash.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/imgui/imgui.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/io/file-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/io/input.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/lib/lib.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/log/log.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/main.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/animation.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/b2GLDraw.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/draw-primitives.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/engine-resources.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/shader-program.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/sprite.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/texture.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/render/vertex-array-object.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/framebuffer-resource.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/renderbuffer-resource.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/res-ptr.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/resource-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/resource.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/resource_types.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/shader-resource.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/resources/texture-resource.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/string/string-id.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/matrix.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/rect.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/transform.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/vector.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/vertex.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/window/display.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/window/window.hxx
)
# Windows export all symbols TODO: use __declspec(dllexport)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(KENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)
set(IMGUI_IMPL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui-impl)

# Kengine interfaces
add_library(kengine-private INTERFACE)
add_library(kengine-public INTERFACE)
add_library(kengine-interface INTERFACE)

# Link external libs
target_link_libraries(
    kengine-private
    INTERFACE SDL3::SDL3-shared
    INTERFACE efsw
    INTERFACE glad
    INTERFACE stb
    INTERFACE $<LINK_LIBRARY:WHOLE_ARCHIVE, spdlog>
    INTERFACE $<LINK_LIBRARY:WHOLE_ARCHIVE, box2d>
)
target_link_libraries(kengine-public INTERFACE glm::glm)
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
if(MSVC)
    target_compile_definitions(
        kengine-private INTERFACE "-D_CRT_SECURE_NO_WARNINGS=1"
    )
    target_compile_definitions(
        kengine-private INTERFACE "-DE_DECLSPEC=__declspec(dllexport)"
    )
    target_compile_definitions(
        kengine-interface INTERFACE "-DE_DECLSPEC=__declspec(dllimport)"
    )
else()
    target_compile_definitions(kengine-public INTERFACE "-DE_DECLSPEC=")
endif()

target_compile_definitions(
    kengine-public
    INTERFACE $<TARGET_PROPERTY:spdlog,INTERFACE_COMPILE_DEFINITIONS>
    INTERFACE $<TARGET_PROPERTY:box2d,INTERFACE_COMPILE_DEFINITIONS>
)

target_compile_options(
    kengine-public
    INTERFACE
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<COMPILE_LANGUAGE:CUDA>>>:/wd4251
        /wd4275>
)

# Include dirs
target_include_directories(kengine-public INTERFACE ${KENGINE_INCLUDE_DIR})
target_include_directories(kengine-private INTERFACE ${IMGUI_IMPL_INCLUDE_DIR})
target_include_directories(
    kengine-public
    INTERFACE $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>
    INTERFACE $<TARGET_PROPERTY:box2d,INTERFACE_INCLUDE_DIRECTORIES>
)

# IMGUI
target_compile_definitions(kengine-public INTERFACE "-DIMGUI_IMPL_API=")

if(KENGINE_IMGUI)
    target_compile_definitions(kengine-public INTERFACE KENGINE_IMGUI)
    target_link_libraries(
        kengine-private INTERFACE $<LINK_LIBRARY:WHOLE_ARCHIVE, imgui>
    )

    target_compile_definitions(
        kengine-interface
        INTERFACE $<TARGET_PROPERTY:imgui,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_include_directories(
        kengine-interface
        INTERFACE $<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>
    )
endif()

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

install(
    TARGETS kengine-static kengine-shared
    DESTINATION ${CMAKE_BINARY_DIR}/dist
    COMPONENT Kengine
)
