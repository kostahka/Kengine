set(GLAD_SOURCES src/glad.c)
set(GLAD_HEADERS include/glad/glad.h include/KHR/khrplatform.h)

add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_HEADERS})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
