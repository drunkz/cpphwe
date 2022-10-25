#ifndef HW_LOG_H
#define HW_LOG_H

#include "hw_singleton.h"
#include "spdlog/async_logger.h"
#include "spdlog/spdlog.h"

#ifndef HW_LOG_INFO_DISABLED
#define HW_LOG_INFO(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::info, __VA_ARGS__)
#else
#define HW_LOG_INFO(...) (void)0
#endif

#ifndef HW_LOG_WARN_DISABLED
#define HW_LOG_WARN(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::warn, __VA_ARGS__)
#else
#define HW_LOG_WARN(...) (void)0
#endif

#ifndef HW_LOG_TRACE_DISABLED
#define HW_LOG_TRACE(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::trace, __VA_ARGS__)
#else
#define HW_LOG_TRACE(...) (void)0
#endif

#ifndef HW_LOG_DEBUG_DISABLED
#define HW_LOG_DEBUG(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::debug, __VA_ARGS__)
#else
#define HW_LOG_DEBUG(...) (void)0
#endif

#ifndef HW_LOG_ERROR_DISABLED
#define HW_LOG_ERROR(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::err, __VA_ARGS__)
#else
#define HW_LOG_ERROR(...) (void)0
#endif

#ifndef HW_LOG_CRITICAL_DISABLED
#define HW_LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(hw::Log::GetInstance().logger, spdlog::level::critical, __VA_ARGS__)
#else
#define HW_LOG_CRITICAL(...) (void)0
#endif

#define IF_TRUE_LOG_ERR_RET_FALSE(V, ...)                                                                                                            \
    if (V) {                                                                                                                                         \
        HW_LOG_ERROR(__VA_ARGS__);                                                                                                                   \
        return false;                                                                                                                                \
    }

#define IF_TRUE_LOG_ERR_RET(V, ...)                                                                                                                  \
    if (V) {                                                                                                                                         \
        HW_LOG_ERROR(__VA_ARGS__);                                                                                                                   \
        return;                                                                                                                                      \
    }

namespace hw {
class Log : public Singleton<Log> {
    friend class Singleton<Log>;

  public:
    std::shared_ptr<spdlog::async_logger> logger;

    bool Log_Init();
    void Log_SetLevel(UINT8 logFileLevel, UINT8 logConsoleLevel);

  private:
    Log() = default;
    virtual ~Log() = default;
};
} // namespace hw

#endif // HW_LOG_H