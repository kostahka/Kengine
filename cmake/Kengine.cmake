# Windows export all symbols TODO: use __declspec(dllexport)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Kengine sources
set(KENGINE_SOURCES
    # cmake-format: sortable
    ${CMAKE_CURRENT_LIST_DIR}/../src/audio/audio.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/audio/audio.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/engine.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/engine.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/event/event.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/event/event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/event/handle-user-event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/event/user-events.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/file-last-modify-listener.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/element-buffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/framebuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/raw-vertex-buffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/render-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/render-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/renderbuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/shader.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/texture.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/graphics/vertex-array.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/handle-file-modify.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/helpers/hash.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_opengl3_loader.h
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_sdl3.h
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_sdlrenderer3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/imgui-impl/imgui_impl_sdlrenderer3.h
    ${CMAKE_CURRENT_LIST_DIR}/../src/io/file-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/io/input.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/log/log.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/log/log.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/opengl/opengl-debug.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/opengl/opengl-debug.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/opengl/opengl.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/opengl/opengl.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/animation.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/b2GLDraw.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/draw-primitives.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/engine-resources.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/shader-program.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/sprite.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/texture.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/render/vertex-array-object.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/engine-resources.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/engine-resources.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/framebuffer-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/picopng.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/renderbuffer-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/resource-manager.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/shader-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/resources/texture-resource.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/string/string-id.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/window/window.cxx
    ${CMAKE_CURRENT_LIST_DIR}/../src/window/window.hxx
)

set(KENGINE_HEADERS
    # cmake-format: sortable
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/audio/audio.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/engine.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/event/event.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/event/user-events.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/file-last-modify-listener.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/game.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/element-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/framebuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/raw-vertex-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/render-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/renderbuffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/shader.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/texture-common.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/texture.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/vertex-array.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/graphics/vertex-buffer.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/helpers/hash.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/io/file-manager.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/io/input.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/log/log.hxx
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
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/rect.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/transform.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/vector.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/units/vertex.hxx
    ${CMAKE_CURRENT_LIST_DIR}/../include/Kengine/window/window.hxx
)

# <<
# Develop engine target
# <<
macro(
    developgame
    TARGET_NAME
    GAME_SOURCES
    GAME_HEADERS
)
    add_library(${TARGET_NAME} SHARED ${GAME_SOURCES} ${GAME_HEADERS})

    if(ANDROID)
        add_library(
            kenginestart-${TARGET_NAME} SHARED ${KENGINE_SOURCES}
                                               ${KENGINE_HEADERS}
        )
    else()
        add_executable(
            kenginestart-${TARGET_NAME} ${KENGINE_SOURCES} ${KENGINE_HEADERS}
        )
    endif()

    target_link_libraries(
        kenginestart-${TARGET_NAME}
        PUBLIC kengine-public
        PRIVATE kengine-private
        INTERFACE kengine-interface
    )

    target_compile_definitions(
        kenginestart-${TARGET_NAME}
        PUBLIC ENGINE_DEV
        PUBLIC ENGINE_GAME_LIB_NAME="${TARGET_NAME}"
    )

    # Enable exports for game library to link
    set_target_properties(
        kenginestart-${TARGET_NAME} PROPERTIES ENABLE_EXPORTS ON
    )

    target_link_libraries(${TARGET_NAME} PRIVATE kenginestart-${TARGET_NAME})

endmacro()
