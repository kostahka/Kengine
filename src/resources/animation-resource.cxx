#include "Kengine/resources/animation-resource.hxx"

#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/log/log.hxx"

#include "imgui.h"

namespace Kengine
{
    animation_resource::animation_resource(std::string_view name)
        : resource(resource_type::animation, name)
    {
    }

    void animation_resource::add_animation(string_id id, std::vector<rect> uvs)
    {
        animations[id] = uvs;
    }

    void animation_resource::remove_animation(string_id id)
    {
        auto anim_it = animations.find(id);
        if (anim_it != animations.end())
        {
            animations.erase(anim_it);
        }
    }

    animation_resource::~animation_resource()
    {
        if (p_count > 0)
        {
            this->unload_data();
        }
    }

    std::size_t animation_resource::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, animations);

        return size;
    }

    std::size_t animation_resource::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        animations.clear();
        size += serialization::read(is, animations);

        return size;
    }

    std::size_t animation_resource::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(animations);

        return size;
    }

    void animation_resource::load_data() {}

    void animation_resource::unload_data() {}

    bool animation_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        static string_id selected_anim{};
        static string_id remove_anim{};

        {
            ImGui::BeginChild("Animations",
                              { 100, 100 },
                              ImGuiChildFlags_ResizeX |
                                  ImGuiChildFlags_ResizeY |
                                  ImGuiChildFlags_Border);

            for (auto& anim : animations)
            {
                ImGui::PushID(anim.first.get_id());
                if (anim.first.empty())
                {
                    if (ImGui::Selectable("ZERO"))
                    {
                        selected_anim = anim.first;
                    }
                }
                else
                {
                    if (ImGui::Selectable(anim.first.get_string()))
                    {
                        selected_anim = anim.first;
                    }
                }

                if (!anim.first.empty() &&
                    (ImGui::SameLine(), ImGui::Button("x")))
                {
                    remove_anim = anim.first;
                }
                ImGui::PopID();
            }

            if (!remove_anim.empty())
            {
                auto remove_it = animations.find(remove_anim);
                if (remove_it != animations.end())
                {
                    animations.erase(remove_it);
                }
                remove_anim = 0;
                edited      = true;
            }
            ImGui::SameLine();
            {
                ImGui::BeginChild("Uvs",
                                  { 100, 100 },
                                  ImGuiChildFlags_ResizeX |
                                      ImGuiChildFlags_ResizeY |
                                      ImGuiChildFlags_Border);

                auto anim_it = animations.find(selected_anim);
                if (anim_it != animations.end())
                {
                    for (auto i = 0U; i < anim_it->second.size(); ++i)
                    {
                        ImGui::PushID(i);
                        std::string uv_name = "uv" + std::to_string(i);
                        if (ImGui::DragFloat4(uv_name.c_str(),
                                              (float*)&anim_it->second[i],
                                              0.001f))
                        {
                            edited = true;
                        }

                        ImGui::PopID();
                    }
                    if (ImGui::Button("-") && anim_it->second.size() > 1)
                    {
                        anim_it->second.resize(anim_it->second.size() - 1);
                        edited = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("+"))
                    {
                        anim_it->second.resize(anim_it->second.size() + 1);
                        edited = true;
                    }
                }

                static char anim_name[100];
                ImGui::InputText(
                    "New animation name", anim_name, sizeof(anim_name));
                if (ImGui::Button("Add animation"))
                {
                    string_id new_id   = hash_string(anim_name);
                    animations[new_id] = {
                        {0, 0, 1, 1}
                    };
                    edited = true;
                }

                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::PopID();
#endif
        return edited;
    }
} // namespace Kengine