#include "log-wnd.hxx"

#include "Kengine/io/input.hxx"
#include "Kengine/log/log.hxx"
#include "imgui.h"
#include <Kengine/window/window.hxx>

#include <array>
#include <cstring>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

std::unordered_map<spdlog::level::level_enum, format_tags>
    imgui_logger_sink::level_colors = {
        { spdlog::level::trace,    format_tags::color_bright_magenta },
        { spdlog::level::debug,    format_tags::color_cyan           },
        { spdlog::level::info,     format_tags::color_bright_green   },
        { spdlog::level::warn,     format_tags::color_bright_yellow  },
        { spdlog::level::err,      format_tags::color_bright_red     },
        { spdlog::level::critical, format_tags::color_red            },
        { spdlog::level::n_levels, format_tags::color_bright_blue    },
};

void imgui_logger_sink::clear_buffers(bool disable_lock)
{
    if (!disable_lock)
        sink_t::mutex_.lock();

    logged_content.clear();
    log_meta_data.clear();
    log_entries_count = 0;
    memory_usage      = 0;

    if (!disable_lock)
        sink_t::mutex_.unlock();
}

void imgui_logger_sink::rebuild_filtered_view()
{
    bool need_rebuild =
        last_filter_level != log_filter_level ||
        memcmp(log_filter.InputBuf, last_filter_text, sizeof(last_filter_text));

    if (need_rebuild)
    {
        filtered_view.clear();

        for (size_t i = 0; i < log_meta_data.size(); ++i)
        {
            if (log_meta_data[i].log_level < log_filter_level)
            {
                continue;
            }

            auto line_begin =
                logged_content.begin() + log_meta_data[i].begin_index +
                log_meta_data[i].substring_ranges.front().substring_begin;
            auto line_end =
                logged_content.begin() + log_meta_data[i].begin_index +
                log_meta_data[i].substring_ranges.back().substring_end;

            if (!log_filter.PassFilter(line_begin, line_end))
            {
                continue;
            }

            filtered_view.push_back(i);
        }

        last_filter_level = log_filter_level;
        memcpy(last_filter_text, log_filter.InputBuf, sizeof(last_filter_text));
    }
}

void imgui_logger_sink::sink_it_(
    const spdlog::details::log_msg& log_message) noexcept
{
    ++log_entries_count;

    spdlog::memory_buf_t formatted_buffer;
    sink_t::formatter_->format(log_message, formatted_buffer);
    std::string formatted_text = fmt::to_string(formatted_buffer);

    bool filter_passing =
        log_message.level >= log_filter_level &&
        log_filter.PassFilter(formatted_text.c_str(),
                              formatted_text.c_str() + formatted_text.size());

    sink_line_content new_line{ log_message.level, logged_content.size() };

    uint32_t color_range_start = log_message.color_range_start;
    uint32_t color_range_end   = log_message.color_range_end;
    uint32_t line_end          = formatted_text.size();

    auto        level_color_it = level_colors.find(log_message.level);
    format_tags level_format   = level_color_it != level_colors.end()
                                     ? level_color_it->second
                                     : format_tags::color_reset;

    new_line.substring_ranges.push_back({
        .substring_begin = 0,
        .substring_end   = color_range_start,
        .format_tag      = format_tags::color_reset,
    });

    new_line.substring_ranges.push_back({
        .substring_begin = color_range_start,
        .substring_end   = color_range_end,
        .format_tag      = level_format,
    });

    new_line.substring_ranges.push_back({
        .substring_begin = color_range_end,
        .substring_end   = line_end,
        .format_tag      = format_tags::color_reset,
    });

    if (filter_passing)
    {
        filtered_view.push_back(log_meta_data.size());
    }
    log_meta_data.push_back(new_line);

    logged_content.append(formatted_text.c_str(),
                          formatted_text.c_str() + formatted_text.size());
}

void imgui_logger_sink::flush_() {};

class SinkLineContent
{
public:
    spdlog::level::level_enum LogLevel; // If n_levels, the message pushed
                                        // counts to the previous pushed line
    int32_t BeginIndex;                 // Base offset into the text buffer

    struct ColorDataRanges
    {
        uint32_t SubStringBegin : 12;
        uint32_t SubStringEnd : 12;
        uint32_t FormatTag : 8;
    };

    ImVector<ColorDataRanges> FormattedStringRanges;
};

void log_wnd::initialize()
{
    Kengine::log::logger->sinks().push_back(logger_sink);
}

void log_wnd::display()
{
    ImGui::PushID(this);

    if (ImGui::Begin(window_name))
    {
        ImGui::Checkbox("Auto-scroll", &enable_autoscrolling);

        ImGui::SameLine();

        if (ImGui::Button("Copy"))
            Kengine::input::clipboard::set_text(
                logger_sink->get_logged_content().c_str());

        ImGui::SameLine();

        if (ImGui::Button("Clear"))
            logger_sink->clear_buffers();

        ImGui::SameLine();

        ImGui::Text("%d messages logged, using %dkb memory",
                    logger_sink->get_log_entries_count(),
                    (logger_sink->get_memory_usage()) / (1024));

        static constexpr auto spdlog_names = SPDLOG_LEVEL_NAMES;
        constexpr auto        log_levels   = []() constexpr
        {
            std::array<const char*, spdlog_names.size()> res{};
            for (auto i = 0; i < spdlog_names.size(); ++i)
            {
                res[i] = (spdlog_names.begin() + i)->data();
            }
            return res;
        }();

        static const auto log_selection_width = [&log_levels]() -> float
        {
            float longest_text_width = 0;
            for (auto log_level_text : log_levels)
            {

                auto text_width = ImGui::CalcTextSize(log_level_text).x;
                if (text_width > longest_text_width)
                    longest_text_width = text_width;
            }

            return longest_text_width + ImGui::GetStyle().FramePadding.x * 2 +
                   ImGui::GetFrameHeight();
        }();

        static const char* active_log_level_text = "Active log level:";
        static const float active_log_level_width =
            ImGui::CalcTextSize(active_log_level_text).x + log_selection_width;
        auto active_log_level_aligment =
            ImGui::GetWindowSize().x -
            (active_log_level_width + ImGui::GetStyle().WindowPadding.x);

        ImGui::SameLine(active_log_level_aligment);
        ImGui::Text("%s", active_log_level_text);

        auto combo_right_aligment =
            ImGui::GetWindowSize().x -
            (log_selection_width + ImGui::GetStyle().WindowPadding.x);
        auto active_log_level = logger_sink->level();

        ImGui::SetNextItemWidth(log_selection_width);
        ImGui::SameLine(combo_right_aligment);
        ImGui::Combo("##ActiveLogLevel",
                     reinterpret_cast<int32_t*>(&active_log_level),
                     log_levels.data(),
                     log_levels.size());
        logger_sink->set_level(active_log_level);

        static const char* log_level_filter_text = "Log level filter:";
        static const float log_level_filter_width =
            ImGui::CalcTextSize(log_level_filter_text).x + log_selection_width;
        auto log_level_filter_aligment =
            ImGui::GetWindowSize().x -
            (log_level_filter_width + ImGui::GetStyle().WindowPadding.x);

        static const char* log_filter_text = "Log filter:";
        static auto log_filter_text_width = ImGui::CalcTextSize(log_filter_text).x +
                                     ImGui::GetStyle().FramePadding.x * 2 +
                                     ImGui::GetFrameHeight();

        // Filter out messages on display
        ImGui::Text("%s", log_filter_text);
        ImGui::SameLine();
        logger_sink->log_filter.Draw(
            "##LogFilter",
            ImGui::GetWindowSize().x -
                (log_filter_text_width + log_level_filter_width +
                 ImGui::GetStyle().WindowPadding.x * 2 +
                 ImGui::GetStyle().FramePadding.x));
        ImGui::SameLine(log_level_filter_aligment);
        ImGui::Text("%s", log_level_filter_text);
        ImGui::SetNextItemWidth(log_selection_width);
        ImGui::SameLine(combo_right_aligment);
        ImGui::Combo("##FilterLogLevel",
                     &logger_sink->log_filter_level,
                     log_levels.data(),
                     log_levels.size());

        ImGui::Separator();
        ImGui::BeginChild("LogTextView",
                          ImVec2(0, 0),
                          false,
                          ImGuiWindowFlags_AlwaysVerticalScrollbar |
                              ImGuiWindowFlags_AlwaysHorizontalScrollbar);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        logger_sink->lock();
        logger_sink->rebuild_filtered_view();

        auto filtered_view  = logger_sink->get_filtered_view();
        auto log_meta_data  = logger_sink->get_log_meta_data();
        auto logged_content = logger_sink->get_logged_content();

        ImGuiListClipper view_clipper;
        view_clipper.Begin(filtered_view.size());
        while (view_clipper.Step())
        {
            int32_t pushed_styles_count = 0;
            for (auto line_index = view_clipper.DisplayStart;
                 line_index < view_clipper.DisplayEnd;
                 ++line_index)
            {
                auto& meta_data = log_meta_data[filtered_view[line_index]];

                for (auto i = 0; i < meta_data.substring_ranges.size(); ++i)
                {
                    switch (meta_data.substring_ranges[i].format_tag)
                    {
                        case format_tags::color_reset:
                            ImGui::PopStyleColor(pushed_styles_count);
                            pushed_styles_count = 0;
                            break;

                        case format_tags::color_black:
                        case format_tags::color_red:
                        case format_tags::color_green:
                        case format_tags::color_yellow:
                        case format_tags::color_blue:
                        case format_tags::color_magenta:
                        case format_tags::color_cyan:
                        case format_tags::color_white:
                        case format_tags::color_bright_black:
                        case format_tags::color_bright_red:
                        case format_tags::color_bright_green:
                        case format_tags::color_bright_yellow:
                        case format_tags::color_bright_blue:
                        case format_tags::color_bright_magenta:
                        case format_tags::color_bright_cyan:
                        case format_tags::color_bright_white:
                        {
                            static const ImVec4 colors_to_vec[]{
                                { 0.0f, 0.0f, 0.0f, 1 }, // COLOR_BLACK
                                { 0.5f, 0.0f, 0.0f, 1 }, // COLOR_RED
                                { 0.0f, 0.7f, 0.0f, 1 }, // COLOR_GREEN
                                { 0.7f, 0.7f, 0.0f, 1 }, // COLOR_YELLOW
                                { 0.0f, 0.0f, 0.7f, 1 }, // COLOR_BLUE
                                { 0.7f, 0.0f, 0.7f, 1 }, // COLOR_MAGENTA
                                { 0.0f, 0.7f, 0.7f, 1 }, // COLOR_CYAN
                                { 0.7f, 0.7f, 0.7f, 1 }, // COLOR_WHITE
                                { 0,    0,    0,    1 }, // COLOR_BRIGHTBLACK
                                { 1,    0,    0,    1 }, // COLOR_BRIGHTRED
                                { 0,    1,    0,    1 }, // COLOR_BRIGHTGREEN
                                { 1,    1,    0,    1 }, // COLOR_BRIGHTYELLOW
                                { 0,    0,    1,    1 }, // COLOR_BRIGHTBLUE
                                { 1,    0,    1,    1 }, // COLOR_BRIGHTMAGENTA
                                { 0,    1,    1,    1 }, // COLOR_BRIGHTCYAN
                                { 1,    1,    1,    1 }, // COLOR_BRIGHTWHITE
                            };
                            ImGui::PushStyleColor(
                                ImGuiCol_Text,
                                colors_to_vec[static_cast<uint32_t>(
                                                  meta_data.substring_ranges[i]
                                                      .format_tag) -
                                              static_cast<uint32_t>(
                                                  format_tags::color_black)]);
                            ++pushed_styles_count;
                        }
                        break;
                    }

                    auto format_range_begin =
                        logged_content.begin() + meta_data.begin_index +
                        meta_data.substring_ranges[i].substring_begin;
                    auto format_range_end =
                        logged_content.begin() + meta_data.begin_index +
                        meta_data.substring_ranges[i].substring_end;
                    ImGui::TextUnformatted(format_range_begin,
                                           format_range_end);
                    if (meta_data.substring_ranges.size() - (i + 1))
                        ImGui::SameLine();
                }
            }
            ImGui::PopStyleColor(pushed_styles_count);
        }
        view_clipper.End();
        ImGui::PopStyleVar();

        logger_sink->unlock();

        if (enable_autoscrolling &&
            ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
    }

    ImGui::End();
    ImGui::PopID();
}