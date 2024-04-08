#pragma once

#include "Kengine/audio/specs.hxx"
#include "Kengine/helpers/spin-lock.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "resource.hxx"

namespace Kengine
{
    class E_DECLSPEC audio_resource : public resource
    {
    public:
        audio_resource(std::string_view name);
        audio_resource(std::filesystem::path f_path, std::string_view name);

        ~audio_resource() override;

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        spin_lock data_lock;

        inline uint8_t* get_data() const { return data; }

        inline uint32_t get_length() const { return length; }

        inline const audio::spec& get_spec() const { return spec; }

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        uint8_t*    data   = nullptr;
        uint32_t    length = 0;
        audio::spec spec;
    };
} // namespace Kengine