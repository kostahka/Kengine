file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(imgui-private INTERFACE)
add_library(imgui-public INTERFACE)
add_library(imgui-interface INTERFACE)

target_include_directories(imgui-public INTERFACE .)
target_include_directories(imgui-interface INTERFACE .)
target_compile_definitions(
    imgui-public INTERFACE "-DIMGUI_API=__declspec(dllexport)"
)
target_compile_definitions(
    imgui-interface INTERFACE "-DIMGUI_API=__declspec(dllimport)"
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_link_libraries(
    imgui
    PRIVATE imgui-private
    PUBLIC imgui-public
)

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
