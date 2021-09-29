//
// Created by aleksandr on 16.04.2021.
//

#include <stdarg.h>

#include <iostream>
#include <string>

#include "Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace logic {

uint64_t Logger::count = 0;

Logger::Logger(const std::string& log) {
//     if (log == "") {
//         auto console = spdlog::stdout_color_mt("console");
//         logger_ = spdlog::get("console");
//     } else {
//         logger_ = spdlog::basic_logger_mt("logger", log);
//     }
    try {
        auto name = "basic_logger" + std::to_string(count);
        auto filename = "basic-log" + std::to_string(count++) + ".txt";
        logger_ = spdlog::basic_logger_mt(name, filename);
    } catch (const spdlog::spdlog_ex &ex) {
            std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

}  // namespace logic
