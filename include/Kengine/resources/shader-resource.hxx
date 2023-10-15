#pragma once

#include "resource.hxx"

#include <memory>
#include <string>
#include <string_view>

namespace Kengine
{
    using std::shared_ptr;
    using std::string;
    using std::string_view;

    class fragment_shader_res : public resource
    {
    public:
        fragment_shader_res(const path& fragment_file);
        fragment_shader_res(const string_view& fragment_code);
        ~fragment_shader_res();

        inline uint32_t get_id() const { return fragment_id; };

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t fragment_id;
    };

    class vertex_shader_res : public resource
    {
    public:
        vertex_shader_res(const path& vertex_file);
        vertex_shader_res(const string_view& vertex_code);
        ~vertex_shader_res();

        inline uint32_t get_id() const { return vertex_id; };

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t vertex_id;
    };

    class geometry_shader_res : public resource
    {
    public:
        geometry_shader_res(const path& geometry_file);
        geometry_shader_res(const string_view& geometry_code);
        ~geometry_shader_res();

        inline uint32_t get_id() const { return geometry_id; };

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        string   code;
        uint32_t geometry_id;
    };

    class shader_res : public resource
    {
    public:
        shader_res(const shared_ptr<vertex_shader_res>&,
                   const shared_ptr<geometry_shader_res>&,
                   const shared_ptr<fragment_shader_res>&);
        shader_res(const shared_ptr<vertex_shader_res>&,
                   const shared_ptr<fragment_shader_res>&);

        shader_res(const string_view& vertex_code,
                   const string_view& fragment_code);
        shader_res(const string_view& vertex_code,
                   const string_view& geometry_code,
                   const string_view& fragment_code);

        shader_res(const path& vertex_path, const path& fragment_path);
        shader_res(const path& vertex_path,
                   const path& geometry_path,
                   const path& fragment_path);

        ~shader_res();

        inline uint32_t get_id() const { return id; };

    protected:
        void load_data() override;
        void unload_data() override;

    private:
        shared_ptr<vertex_shader_res>   vertex_res;
        shared_ptr<geometry_shader_res> geometry_res;
        shared_ptr<fragment_shader_res> fragment_res;

        uint32_t id;
    };

} // namespace Kengine