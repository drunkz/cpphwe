#include "include/hw_log.h"
#include "spdlog/async.h"
#include "spdlog/common.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

namespace hw {

bool Log::Log_Init() {
    try {
        spdlog::init_thread_pool(8192, 1);
        auto daily_file_sink = std::make_shared<spdlog::sinks::daily_file_format_sink_mt>("log/%Y-%m-%d.txt", 0, 0);
        daily_file_sink->set_pattern("%H:%M:%S.%e %t %s:%!:%# [%l] %v");
        daily_file_sink->set_level(spdlog::level::trace);
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%Y-%m-%d %H:%M:%S.%e %^%l%$ %v");
        console_sink->set_level(spdlog::level::trace);
        spdlog::sinks_init_list sink_list = {daily_file_sink, console_sink};
        logger = std::make_shared<spdlog::async_logger>("logger", sink_list, spdlog::thread_pool());
        logger->set_level(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    } catch (const std::exception& ex) {
        std::cout << "日志系统初始化失败: " << ex.what() << std::endl;
        return false;
    }
    return true;
}

void Log::Log_SetLevel(UINT8 logFileLevel, UINT8 logConsoleLevel) {
    logger->sinks()[0]->set_level(spdlog::level::level_enum(logFileLevel));
    logger->sinks()[1]->set_level(spdlog::level::level_enum(logConsoleLevel));
}

} // namespace hw
