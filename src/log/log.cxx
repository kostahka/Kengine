#include "Kengine/log/log.hxx"
#include "log.hxx"
#include <memory>

#ifdef __ANDROID__
 #include "spdlog/sinks/android_sink.h"
#else
 #include "spdlog/sinks/basic_file_sink.h"
 #include "spdlog/sinks/stdout_color_sinks.h"
#endif

namespace Kengine::log
{
    constexpr const char* logger_name       = "kengine-logger";
    constexpr const char* fatal_logger_name = "kengine-fatal-logger";

    std::shared_ptr<spdlog::logger> logger{ nullptr };
    std::shared_ptr<spdlog::logger> fatal_logger{ nullptr };

#ifdef __ANDROID__
    static std::shared_ptr<spdlog::sinks::android_sink_st> android_sink{};
#else
    static std::shared_ptr<spdlog::sinks::basic_file_sink_st>   file_sink{};
    static std::shared_ptr<spdlog::sinks::stdout_color_sink_st> console_sink{};
#endif

    bool initialize()
    {
#ifdef __ANDROID__
        // Logger for app fatal
        android_sink =
            std::make_shared<spdlog::sinks::android_sink_st>("kengine");

        fatal_logger =
            std::make_shared<spdlog::logger>(fatal_logger_name, android_sink);
        fatal_logger->set_level(spdlog::level::debug);
        fatal_logger->enable_backtrace(20);

        spdlog::register_logger(fatal_logger);

        // Debug logger
        logger = std::make_shared<spdlog::logger>(logger_name, android_sink);

        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(logger);
#else
        // Logger for app fatal
        file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_st>("kengine.log");

        fatal_logger =
            std::make_shared<spdlog::logger>(fatal_logger_name, file_sink);
        fatal_logger->set_level(spdlog::level::off);
        fatal_logger->enable_backtrace(20);

        spdlog::register_logger(fatal_logger);

        // Debug logger
        console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();

        logger = std::make_shared<spdlog::logger>(logger_name, console_sink);

        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(logger);
#endif

        return true;
    }

    void shutdown()
    {
        spdlog::shutdown();
    }
} // namespace Kengine::log