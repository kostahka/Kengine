#pragma once

#include "spdlog/spdlog.h"

#include <cstdlib>
#include <memory>

#if defined(NDEBUG) && !defined(ENGINE_DEV)
 #define KENGINE_TRACE(...) (void)0
 #define KENGINE_DEBUG(...)                                                    \
     {                                                                         \
         Kengine::log::fatal_logger->debug(__VA_ARGS__);                       \
     }
 #define KENGINE_INFO(...)                                                     \
     {                                                                         \
         Kengine::log::fatal_logger->info(__VA_ARGS__);                        \
     }
 #define KENGINE_WARN(...)                                                     \
     {                                                                         \
         Kengine::log::fatal_logger->warn(__VA_ARGS__);                        \
     }
 #define KENGINE_ERROR(...)                                                    \
     {                                                                         \
         Kengine::log::fatal_logger->error(__VA_ARGS__);                       \
     }
 #define KENGINE_FATAL(...)                                                    \
     {                                                                         \
         Kengine::log::fatal_logger->critical(__VA_ARGS__);                    \
         Kengine::log::fatal_logger->dump_backtrace();                         \
     }
 #define KENGINE_ASSERT(EX, ...) (void)0
#else
 #define KENGINE_TRACE(...)                                                    \
     {                                                                         \
         Kengine::log::logger->trace(__VA_ARGS__);                             \
     }
 #define KENGINE_DEBUG(...)                                                    \
     {                                                                         \
         Kengine::log::logger->debug(__VA_ARGS__);                             \
         Kengine::log::fatal_logger->debug(__VA_ARGS__);                       \
     }
 #define KENGINE_INFO(...)                                                     \
     {                                                                         \
         Kengine::log::logger->info(__VA_ARGS__);                              \
         Kengine::log::fatal_logger->info(__VA_ARGS__);                        \
     }
 #define KENGINE_WARN(...)                                                     \
     {                                                                         \
         Kengine::log::logger->warn(__VA_ARGS__);                              \
         Kengine::log::fatal_logger->warn(__VA_ARGS__);                        \
     }
 #define KENGINE_ERROR(...)                                                    \
     {                                                                         \
         Kengine::log::logger->error(__VA_ARGS__);                             \
         Kengine::log::fatal_logger->error(__VA_ARGS__);                       \
     }
 #define KENGINE_FATAL(...)                                                    \
     {                                                                         \
         Kengine::log::logger->critical(__VA_ARGS__);                          \
         Kengine::log::fatal_logger->critical(__VA_ARGS__);                    \
         Kengine::log::fatal_logger->dump_backtrace();                         \
     }
 #define KENGINE_ASSERT(EX, msg)                                               \
     if (!(EX))                                                                \
     {                                                                         \
         Kengine::log::logger->critical(                                       \
             "Assert fail at line {}, at file {}, message: {}",                \
             __LINE__,                                                         \
             __FILE__,                                                         \
             (msg));                                                           \
         abort();                                                              \
     }
#endif

namespace Kengine::log
{
    extern std::shared_ptr<spdlog::logger> logger;
    extern std::shared_ptr<spdlog::logger> fatal_logger;
} // namespace Kengine::log