file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC .)
set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
