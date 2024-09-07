cmake_minimum_required(VERSION 3.15.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Kengine-demo VERSION 0.0.0)

include(../cmake/Kengine.cmake)

set(KENGINE_DEMO_GAME_SOURCES
    # cmake-format: sortable
    src/demo-game.cxx src/player-controller-component.cxx
    src/player-controller-system.cxx
)

set(KENGINE_DEMO_GAME_HEADERS
    # cmake-format: sortable
    include/demo-game.hxx include/player-controller-component.hxx
    include/player-controller-system.hxx
)

if(ANDROID)
    add_library(
        kengine-demo-static ${KENGINE_DEMO_GAME_SOURCES}
                            ${KENGINE_DEMO_GAME_HEADERS}
    )
else()
    add_executable(
        kengine-demo-static ${KENGINE_DEMO_GAME_SOURCES}
                            ${KENGINE_DEMO_GAME_HEADERS}
    )
endif()

developgame(
    kengine-demo-dev "${KENGINE_DEMO_GAME_SOURCES}"
    "${KENGINE_DEMO_GAME_HEADERS}"
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
