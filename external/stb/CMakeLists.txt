set(STB_SOURCES src/stb_image_impl.c)
set(STB_HEADERS include/stb_image.h)

add_library(stb STATIC ${STB_SOURCES} ${STB_HEADERS})
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_target_properties(stb PROPERTIES POSITION_INDEPENDENT_CODE ON)
