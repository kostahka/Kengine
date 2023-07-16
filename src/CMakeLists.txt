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
    event/event-engine.hxx
    event/event.cxx
    event/handle-user-event.hxx
    event/user-events.cxx
    file-last-modify-listener.cxx
    handle-file-modify.hxx
    io/file-manager.cxx
    io/input.cxx
    render/animation.cxx
    render/b2GLDraw.cxx
    render/draw-primitives.cxx
    render/engine-resources.cxx
    render/opengl-error.cxx
    render/opengl-error.hxx
    render/picopng.hxx
    render/shader-program.cxx
    render/sprite.cxx
    render/texture.cxx
    render/vertex-array-object.cxx
    transform3d.cxx
)

set(KENGINE_HEADERS
    # cmake-format: sortable
    ../include/Kengine/audio/audio.hxx
    ../include/Kengine/engine.hxx
    ../include/Kengine/event/event.hxx
    ../include/Kengine/event/user-events.hxx
    ../include/Kengine/file-last-modify-listener.hxx
    ../include/Kengine/io/file-manager.hxx
    ../include/Kengine/io/input.hxx
    ../include/Kengine/render/animation.hxx
    ../include/Kengine/render/b2GLDraw.hxx
    ../include/Kengine/render/color.hxx
    ../include/Kengine/render/draw-primitives.hxx
    ../include/Kengine/render/engine-resources.hxx
    ../include/Kengine/render/shader-program.hxx
    ../include/Kengine/render/sprite.hxx
    ../include/Kengine/render/texture.hxx
    ../include/Kengine/render/vertex-array-object.hxx
    ../include/Kengine/render/vertex.hxx
    ../include/Kengine/transform3d.hxx
)

set(KENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)

# Develop engine target
if(ANDROID)
    add_library(kengine SHARED ${KENGINE_SOURCES} ${KENGINE_HEADERS})
else()
    add_executable(kengine ${KENGINE_SOURCES} ${KENGINE_HEADERS})
endif()

#Link external libs
target_link_libraries(
    kengine
    PRIVATE SDL3::SDL3-static
    PRIVATE efsw
    PRIVATE glad
    PUBLIC imgui
    PUBLIC glm::glm
    PUBLIC box2d
)
if(ANDROID)
    target_link_libraries(
        engine
        PUBLIC android
               log
               EGL
               GLESv3
    )
endif()

# Compile definitions
target_compile_definitions(
    kengine
    PUBLIC ENGINE_DEV
    PUBLIC ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME}"
    PUBLIC IMGUI_IMPL_OPENGL_ES3
)
if(WIN32)
    target_compile_definitions(
        kengine PRIVATE "-DE_DECLSPEC=__declspec(dllexport)"
    )
else()
    target_compile_definitions(kengine PRIVATE "-DE_DECLSPEC=")
endif()

# Include dirs
target_include_directories(kengine PUBLIC ${KENGINE_INCLUDE_DIR})

# Enable exports for game library to link
set_target_properties(kengine PROPERTIES ENABLE_EXPORTS ON)

# Shared engine lib
add_library(kengine-shared SHARED ${KENGINE_SOURCES} ${KENGINE_HEADERS})

# Link external libs
target_link_libraries(
    kengine-shared
    PRIVATE SDL3::SDL3-static
    PRIVATE efsw
    PRIVATE glad
    PUBLIC imgui
    PUBLIC glm::glm
    PUBLIC box2d
)
if(ANDROID)
    target_link_libraries(
        kengine-shared
        PUBLIC android
               log
               EGL
               GLESv3
    )
endif()

# Compile definitions
target_compile_definitions(kengine-shared PUBLIC IMGUI_IMPL_OPENGL_ES3)
if(WIN32)
    target_compile_definitions(
        kengine-shared PRIVATE "-DE_DECLSPEC=__declspec(dllexport)"
    )
else()
    target_compile_definitions(kengine-shared PRIVATE "-DE_DECLSPEC=")
endif()

# Include dirs
target_include_directories(kengine-shared PUBLIC ${KENGINE_INCLUDE_DIR})

# PIC for shared lib
set_property(TARGET kengine-shared PROPERTY POSITION_INDEPENDENT_CODE ON)

# Static engine lib
add_library(kengine-static STATIC ${KENGINE_SOURCES} ${KENGINE_HEADERS})

#Link external libs
target_link_libraries(
    kengine-static
    PRIVATE SDL3::SDL3-static
    PRIVATE efsw
    PRIVATE glad
    PUBLIC imgui
    PUBLIC glm::glm
    PUBLIC box2d
)
if(ANDROID)
    target_link_libraries(
        kengine-static
        PUBLIC android
               log
               EGL
               GLESv3
    )
endif()

# Compile definitions
target_compile_definitions(kengine-static PUBLIC IMGUI_IMPL_OPENGL_ES3)
if(WIN32)
    target_compile_definitions(
        kengine-static PRIVATE "-DE_DECLSPEC=__declspec(dllexport)"
    )
else()
    target_compile_definitions(kengine-static PRIVATE "-DE_DECLSPEC=")
endif()

# Include dirs
target_include_directories(kengine-static PUBLIC ${KENGINE_INCLUDE_DIR})

# PIC for static lib
set_property(TARGET kengine-static PROPERTY POSITION_INDEPENDENT_CODE ON)
