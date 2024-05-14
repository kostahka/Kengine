#include "assets-browser.hxx"

#include "editor.hxx"

#include "Kengine/game.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/resources/animation-resource.hxx"
#include "Kengine/resources/audio-resource.hxx"
#include "Kengine/resources/font-material-resource.hxx"
#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/framebuffer-resource.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/resources/material-resource.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/shader-resource.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/resources/texture-resource.hxx"
#include "Kengine/scene/scene.hxx"

#include <memory>

assets_browser::assets_browser() {}

void assets_browser::display()
{
    ImGui::PushID(this);
    assets_file_browser.Display();

    auto current_game = editor::instance->current_game;

    {
        static ImVec2 create_asset_popup_pos;
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
            assets_file_browser.IsHovered())
        {
            std::filesystem::path file_path = assets_file_browser.GetSelected();
            if (file_path.has_extension())
            {
                if (file_path.extension() == ".kpkg")
                {
                    editor::instance->current_res =
                        Kengine::resource_manager::load_resource(file_path);
                }
                else if (file_path.extension() == ".ksc")
                {
                    if (current_game)
                    {
                        editor::instance->set_game_scene(file_path);
                    }
                }
            }
        }
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
            assets_file_browser.IsHovered())
        {
            ImGui::OpenPopup("create_asset");
            create_asset_popup_pos = ImGui::GetMousePos();
        }

        ImGui::SetNextWindowPos(create_asset_popup_pos);
        if (ImGui::BeginPopup("create_asset"))
        {
            auto current_game = editor::instance->current_game;
            if (current_game)
            {
                if (ImGui::Selectable("Create scene"))
                {
                    create_resource_modal = true;
                    create_file_type      = file_type::scene;
                }
                if (ImGui::BeginMenu("Create resource"))
                {
                    if (ImGui::Selectable("Material"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::material;
                    }
                    if (ImGui::Selectable("Sprite material"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type =
                            Kengine::resource_type::sprite_material;
                    }
                    if (ImGui::Selectable("Gui material"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::gui_material;
                    }
                    if (ImGui::Selectable("Framebuffer"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::framebuffer;
                    }
                    if (ImGui::Selectable("Renderbuffer"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::renderbuffer;
                    }
                    if (ImGui::Selectable("Texture"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type       = Kengine::resource_type::texture;
                    }
                    if (ImGui::Selectable("Vertex shader"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::vertex_shader;
                    }
                    if (ImGui::Selectable("Fragment shader"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type =
                            Kengine::resource_type::fragment_shader;
                    }
                    if (ImGui::Selectable("Geometry shader"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type =
                            Kengine::resource_type::geometry_shader;
                    }
                    if (ImGui::Selectable("Shader program"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type =
                            Kengine::resource_type::shader_program;
                    }
                    if (ImGui::Selectable("Audio"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type       = Kengine::resource_type::audio;
                    }
                    if (ImGui::Selectable("Font"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type       = Kengine::resource_type::font;
                    }
                    if (ImGui::Selectable("Animation"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::animation;
                    }
                    if (ImGui::Selectable("Font material"))
                    {
                        create_resource_modal = true;
                        create_file_type      = file_type::resource;
                        create_res_type = Kengine::resource_type::font_material;
                    }
                    ImGui::EndMenu();
                }
            }
            else
            {
                ImGui::Text("No opened game");
            }

            ImGui::EndPopup();
        }

        if (create_resource_modal)
        {
            ImGui::OpenPopup("create_resource");
            ImGui::SetNextWindowSize({ 400, 100 });
        }

        if (ImGui::BeginPopupModal("create_resource",
                                   &create_resource_modal,
                                   ImGuiWindowFlags_NoResize))
        {
            ImGui::InputText("Filename", new_filename, new_filename_size);
            if (ImGui::Button("Apply"))
            {
                create_resource_modal = false;
                std::filesystem::path new_file_path =
                    assets_file_browser.GetPwd();

                std::string filename{ new_filename };

                if (create_file_type == file_type::scene)
                {
                    auto current_game = editor::instance->current_game;

                    if (current_game)
                    {
                        new_file_path = new_file_path / (filename + ".ksc");
                        auto new_file = Kengine::file_manager::open_file(
                            new_file_path, std::ios_base::out);

                        if (new_file)
                        {
                            auto new_scene =
                                std::make_shared<Kengine::scene>(current_game);

                            std::ostream ofile{ new_file.get() };
                            Kengine::serialization::write(ofile, *new_scene);
                            assets_file_browser.UpdateFileRecords();
                        }
                    }
                }
                else if (create_file_type == file_type::resource)
                {
                    Kengine::res_ptr<Kengine::resource> new_res = nullptr;
                    new_file_path = new_file_path / (filename + ".kpkg");
                    if (create_res_type == Kengine::resource_type::material)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::material_resource>(new_file_path,
                                                        filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::sprite_material)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::sprite_material_resource>(new_file_path,
                                                               filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::gui_material)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::gui_material_resource>(new_file_path,
                                                            filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::framebuffer)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::framebuffer_resource>(new_file_path,
                                                           filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::renderbuffer)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::renderbuffer_resource>(new_file_path,
                                                            filename);
                    }
                    else if (create_res_type == Kengine::resource_type::texture)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::texture_resource>(new_file_path, filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::fragment_shader)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::fragment_shader_res>(new_file_path,
                                                          filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::geometry_shader)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::geometry_shader_res>(new_file_path,
                                                          filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::vertex_shader)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::vertex_shader_res>(new_file_path,
                                                        filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::shader_program)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::shader_res>(new_file_path, filename);
                    }
                    else if (create_res_type == Kengine::resource_type::audio)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::audio_resource>(new_file_path, filename);
                    }
                    else if (create_res_type == Kengine::resource_type::font)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::font_resource>(new_file_path, filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::animation)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::animation_resource>(new_file_path,
                                                         filename);
                    }
                    else if (create_res_type ==
                             Kengine::resource_type::font_material)
                    {
                        new_res = Kengine::make_resource_from_file<
                            Kengine::font_material_resource>(new_file_path,
                                                             filename);
                    }

                    Kengine::resource_manager::save_resource(new_res);

                    assets_file_browser.UpdateFileRecords();

                    editor::instance->current_res = new_res;
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::PopID();
}
