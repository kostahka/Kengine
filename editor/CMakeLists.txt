set(KENGINE_EDITOR_SOURCES
    # cmake-format: sortable
    src/assets-browser.cxx
    src/editor.cxx
    src/game-properties-wnd.cxx
    src/game-wnd.cxx
    src/log-wnd.cxx
    src/object-properties-wnd.cxx
    src/resource-wnd.cxx
    src/scene-objects-wnd.cxx
    src/scene-properties-wnd.cxx
)

set(KENGINE_EDITOR_HEADERS
    # cmake-format: sortable
    include/assets-browser.hxx
    include/editor.hxx
    include/game-properties-wnd.hxx
    include/game-wnd.hxx
    include/imgui-filebrowser/imfilebrowser.h
    include/log-wnd.hxx
    include/object-properties-wnd.hxx
    include/resource-wnd.hxx
    include/scene-objects-wnd.hxx
    include/scene-properties-wnd.hxx
)

add_executable(
    kengine-editor ${KENGINE_EDITOR_SOURCES} ${KENGINE_EDITOR_HEADERS}
)

target_include_directories(
    kengine-editor PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(kengine-editor PRIVATE Kengine::kengine-shared)
target_link_libraries(kengine-editor PRIVATE efsw)

if(WIN32)
    add_custom_target(
        copy_kengine_shared_to_editor ALL
        COMMAND
            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kengine-shared>
            $<TARGET_FILE_DIR:kengine-editor>/
        COMMAND
            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:kengine-editor>/
        COMMAND
            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            $<TARGET_FILE_DIR:kengine-editor>/
        COMMENT
            "Copying kengine-shared.dll into editor directory: [$<TARGET_FILE_DIR:kengine-editor>]"
    )
endif()
