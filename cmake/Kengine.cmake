# <<
# Develop engine target
# <<
macro(
    developgame
    TARGET_NAME
    GAME_SOURCES
    GAME_HEADERS
)
    add_library(${TARGET_NAME} SHARED ${GAME_SOURCES};${GAME_HEADERS})

    target_link_libraries(${TARGET_NAME} PRIVATE Kengine::kengine-shared)

    target_compile_definitions(${TARGET_NAME} PUBLIC ENGINE_DEV)
endmacro()
