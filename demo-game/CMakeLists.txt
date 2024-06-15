include(../cmake/Kengine.cmake)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(${CMAKE_EXE_LINKER_FLAGS} "${CMAKE_EXE_LINKER_FLAGS} -ld64")
    set(${CMAKE_SHARED_LINKER_FLAGS} "${CMAKE_SHARED_LINKER_FLAGS} -ld64")
    set(${CMAKE_STATIC_LINKER_FLAGS} "${CMAKE_STATIC_LINKER_FLAGS} -ld64")
    set(${CMAKE_MODULE_LINKER_FLAGS} "${CMAKE_MODULE_LINKER_FLAGS} -ld64")
endif()

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

developgame(
    kengine-demo-dev ${KENGINE_DEMO_GAME_SOURCES} ${KENGINE_DEMO_GAME_HEADERS}
)

target_include_directories(
    kengine-demo-static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(
    kengine-demo-dev PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(kengine-demo-static PRIVATE Kengine::kengine-static)

add_custom_target(
    demo_game_copy_assets_shaders ALL
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/
        $<TARGET_FILE_DIR:kengine-demo-static>/assets
    COMMENT
        "Copying shaders and assets into game directory: [$<TARGET_FILE_DIR:kengine-demo-static>]"
)
