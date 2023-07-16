file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB IMGUI_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_link_libraries(imgui PRIVATE SDL3::SDL3-static)
target_include_directories(imgui PUBLIC .)
