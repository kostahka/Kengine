#pragma once

#include <imgui.h>
#include <memory>
#include <spdlog/sinks/base_sink.h>

#include <cstdint>
#include <mutex>
#include <unordered_map>

enum class format_tags : uint32_t
{
    color_reset,

    color_black,
    color_red,
    color_green,
    color_yellow,
    color_blue,
    color_magenta,
    color_cyan,
    color_white,

    color_bright_black,
    color_bright_red,
    color_bright_green,
    color_bright_yellow,
    color_bright_blue,
    color_bright_magenta,
    color_bright_cyan,
    color_bright_white,
};

struct sink_line_content
{
    struct color_data_ranges
    {
        uint32_t    substring_begin : 12;
        uint32_t    substring_end : 12;
        format_tags format_tag : 8;
    };

    spdlog::level::level_enum   log_level;
    int32_t                     begin_index;
    ImVector<color_data_ranges> substring_ranges;
};

class imgui_logger_sink : public spdlog::sinks::base_sink<std::mutex>
{
    using sink_t = spdlog::sinks::base_sink<std::mutex>;

public:
    void clear_buffers(bool disable_lock = false);
    void rebuild_filtered_view();

    const ImGuiTextBuffer& get_logged_content() const { return logged_content; }

    uint32_t get_log_entries_count() const { return log_entries_count; }

    uint32_t get_memory_usage() const { return memory_usage; }

    auto& get_log_meta_data() const { return log_meta_data; }

    auto& get_filtered_view() const { return filtered_view; }

    void lock() { sink_t::mutex_.lock(); }

    void unlock() { sink_t::mutex_.unlock(); }

    ImGuiTextFilter log_filter;
    int32_t         log_filter_level = spdlog::level::trace;

protected:
    void sink_it_(const spdlog::details::log_msg& log_message) noexcept final;
    void flush_() final;

private:
    ImGuiTextBuffer                logged_content;
    std::vector<sink_line_content> log_meta_data;
    ImVector<int32_t>              filtered_view;
    uint32_t                       log_entries_count = 0;
    uint32_t                       memory_usage      = 0;

    int32_t last_filter_level = spdlog::level::trace;
    decltype(ImGuiTextFilter::InputBuf) last_filter_text{};

    static std::unordered_map<spdlog::level::level_enum, format_tags>
        level_colors;
};

class log_wnd
{
public:
    static constexpr const char* window_name = "Logs";

    void initialize();
    void display();

private:
    std::shared_ptr<imgui_logger_sink> logger_sink =
        std::make_shared<imgui_logger_sink>();

    bool enable_autoscrolling = true;
};