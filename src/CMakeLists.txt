include(../cmake/Kengine.cmake)

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
