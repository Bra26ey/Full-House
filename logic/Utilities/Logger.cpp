//
// Created by aleksandr on 16.04.2021.
//

#include<stdarg.h>

#include <string>
#include "Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

Logger::Logger(const std::string& log) {
    if (log == "") {
        auto console = spdlog::stdout_color_mt("console");
        logger_ = spdlog::get("console");
    } else {
        logger_ = spdlog::basic_logger_mt("logger", log);
    }
}



