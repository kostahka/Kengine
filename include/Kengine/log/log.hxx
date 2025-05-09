#pragma once

#include "spdlog/spdlog.h"

#include <memory>
#include <ostream>

#if defined(NDEBUG) && !defined(ENGINE_DEV)
 #define KENGINE_TRACE(...) (void)0
 #define KENGINE_DEBUG(...) (void)0

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
 #define KENGINE_ASSERT(EX, ...)      (void)0
 #define KENGINE_ASSERT_WARN(EX, ...) (void)0
#else
 #define KENGINE_TRACE(...)                                                    \
     {                                                                         \
         Kengine::log::logger->trace(__VA_ARGS__);                             \
     }
 #define KENGINE_DEBUG(...)                                                    \
     {                                                                         \
         Kengine::log::logger->debug(__VA_ARGS__);                             \
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
 #define KENGINE_ASSERT(EX, ...)                                               \
     if (!(EX))                                                                \
     {                                                                         \
         Kengine::log::logger->critical(                                       \
             "Assert fail at line {}, at file {}", __LINE__, __FILE__);        \
         Kengine::log::logger->critical(__VA_ARGS__);                          \
         abort();                                                              \
     }
 #define KENGINE_ASSERT_WARN(EX, ...)                                          \
     if (!(EX))                                                                \
     {                                                                         \
         Kengine::log::logger->warn(                                           \
             "Assert fail at line {}, at file {}", __LINE__, __FILE__);        \
         Kengine::log::logger->warn(__VA_ARGS__);                              \
     }
#endif

namespace Kengine::log
{
    E_DECLSPEC extern std::shared_ptr<spdlog::logger> logger;
    E_DECLSPEC extern std::shared_ptr<spdlog::logger> fatal_logger;
} // namespace Kengine::log