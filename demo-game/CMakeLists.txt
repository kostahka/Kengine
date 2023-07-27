set(KENGINE_DEMO_GAME_SOURCES # cmake-format: sortable
                              src/demo-game.cxx
)

set(KENGINE_DEMO_GAME_HEADERS # cmake-format: sortable
                              include/demo-game.hxx
)

add_executable(
    kengine-demo-static ${KENGINE_DEMO_GAME_SOURCES}
                        ${KENGINE_DEMO_GAME_HEADERS}
)

target_include_directories(
    kengine-demo-static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(kengine-demo-static PRIVATE Kengine::kengine-static)
