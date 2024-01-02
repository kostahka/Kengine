set(KENGINE_EDITOR_SOURCES # cmake-format: sortable
                           src/editor.cxx
)

set(KENGINE_EDITOR_HEADERS # cmake-format: sortable
                           include/editor.hxx
)

add_executable(
    kengine-editor ${KENGINE_EDITOR_SOURCES} ${KENGINE_EDITOR_HEADERS}
)

target_include_directories(
    kengine-editor PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(kengine-editor PRIVATE Kengine::kengine-shared)

if(WIN32)
    add_custom_target(
        copy_kengine_shared_to_editor ALL
        COMMAND
            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kengine-shared>
            $<TARGET_FILE_DIR:kengine-editor>/
        COMMENT
            "Copying kengine-shared.dll into editor directory: [$<TARGET_FILE_DIR:kengine-editor>]"
    )
endif()
