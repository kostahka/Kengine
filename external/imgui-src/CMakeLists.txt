file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(imgui PUBLIC .)

if(MSVC)
    target_compile_definitions(
        imgui PRIVATE "-DIMGUI_API=__declspec(dllexport)"
    )
    target_compile_definitions(
        imgui INTERFACE "-DIMGUI_API=__declspec(dllimport)"
    )
else()
    target_compile_definitions(imgui PRIVATE "-DIMGUI_API=")
    target_compile_definitions(imgui INTERFACE "-DIMGUI_API=")
endif()

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
