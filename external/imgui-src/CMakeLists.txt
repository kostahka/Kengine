file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(imgui PUBLIC .)

if(WIN32 OR CYGWIN)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                 "GNU"
    )
        target_compile_definitions(
            imgui PRIVATE "-DIMGUI_API=__attribute__ ((dllexport))"
        )
        target_compile_definitions(
            imgui INTERFACE "-DIMGUI_API=__attribute__ ((dllimport))"
        )
    else()
        target_compile_definitions(
            imgui PRIVATE "-DIMGUI_API=__declspec(dllexport)"
        )
        target_compile_definitions(
            imgui INTERFACE "-DIMGUI_API=__declspec(dllimport)"
        )
    endif()
else()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.0)
        target_compile_definitions(
            imgui
            PUBLIC "-DIMGUI_API=__attribute__ ((visibility (\"default\")))"
        )
    else()
        target_compile_definitions(imgui PUBLIC "-DIMGUI_API=")
    endif()
endif()

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
