//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_FILELOGGER_H
#define FULL_HOUSE_FILELOGGER_H

#include <fstream>
#include "Logger.h"

class FileLogger: public Logger {
public:
    void Read() override;
    void Write() override;

private:
    std::fstream file_;
};

#endif //FULL_HOUSE_FILELOGGER_H
