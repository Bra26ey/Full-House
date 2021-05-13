//
// Created by aleksandr on 16.04.2021.
//

#ifndef FULL_HOUSE_LOGGER_H
#define FULL_HOUSE_LOGGER_H

#include <string>
#include <fstream>
#include "spdlog/spdlog.h"

class Logger {
public:
    explicit Logger(const std::string& log = "");

    template<typename... Args>
    void Log(const std::string& message, Args... args) {
        logger_->info(message, args...);
    }


private:
    std::ifstream log_file;
    std::shared_ptr<spdlog::logger> logger_;

    static uint64_t count;
};

#endif //FULL_HOUSE_LOGGER_H
