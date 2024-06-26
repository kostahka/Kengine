#pragma once

#include "res-ptr.hxx"
#include "resource.hxx"

#include <string>
#include <string_view>
#include <unordered_map>

namespace Kengine
{
    using std::string;
    using std::string_view;

    class E_DECLSPEC fragment_shader_res : public resource
    {
    public:
        fragment_shader_res(std::string_view name);
        fragment_shader_res(const path& fragment_file, std::string_view name);
        fragment_shader_res(const string_view& fragment_code,
                            std::string_view   name);
        ~fragment_shader_res();

        inline uint32_t get_id() const { return fragment_id; };

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t fragment_id;
    };

    class E_DECLSPEC vertex_shader_res : public resource
    {
    public:
        vertex_shader_res(std::string_view name);
        vertex_shader_res(const path& vertex_file, std::string_view name);
        vertex_shader_res(const string_view& vertex_code,
                          std::string_view   name);
        ~vertex_shader_res();

        inline uint32_t get_id() const { return vertex_id; };

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t vertex_id;
    };

    class E_DECLSPEC geometry_shader_res : public resource
    {
    public:
        geometry_shader_res(std::string_view name);
        geometry_shader_res(const path& geometry_file, std::string_view name);
        geometry_shader_res(const string_view& geometry_code,
                            std::string_view   name);
        ~geometry_shader_res();

        inline uint32_t get_id() const { return geometry_id; };

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t geometry_id;
    };

    class E_DECLSPEC shader_res : public resource
    {
    public:
        shader_res(std::string_view name);
        shader_res(const res_ptr<vertex_shader_res>&,
                   const res_ptr<geometry_shader_res>&,
                   const res_ptr<fragment_shader_res>&,
                   std::string_view name);
        shader_res(const res_ptr<vertex_shader_res>&,
                   const res_ptr<fragment_shader_res>&,
                   std::string_view name);

        shader_res(const string_view& vertex_code,
                   const string_view& fragment_code,
                   std::string_view   name);
        shader_res(const string_view& vertex_code,
                   const string_view& geometry_code,
                   const string_view& fragment_code,
                   std::string_view   name);

        shader_res(const path&      vertex_path,
                   const path&      fragment_path,
                   std::string_view name);
        shader_res(const path&      vertex_path,
                   const path&      geometry_path,
                   const path&      fragment_path,
                   std::string_view name);

        ~shader_res() override;

        void set_uniform_block_binding(std::string_view name, uint32_t binding);

        inline uint32_t get_id() const { return id; };

        inline const std::unordered_map<string_id, uint32_t>&
        get_uniform_locations() const
        {
            return uniform_locations;
        }

        inline uint32_t get_uniform_location(string_id id) const
        {
            auto loc_it = uniform_locations.find(id);
            if (loc_it != uniform_locations.end())
            {
                return loc_it->second;
            }
            return 0;
        }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        res_ptr<vertex_shader_res>   vertex_res;
        res_ptr<geometry_shader_res> geometry_res;
        res_ptr<fragment_shader_res> fragment_res;

        std::unordered_map<string_id, uint32_t>   uniform_locations;
        std::unordered_map<std::string, uint32_t> uniform_block_bindings;

        uint32_t id;
    };

} // namespace Kengine