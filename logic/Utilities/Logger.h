//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_LOGGER_H
#define FULL_HOUSE_LOGGER_H

#include "HandConfiguration.h"

struct Logger {
    virtual void Read() = 0;
    virtual void Write() = 0;

    HandConfiguration config;
};

#endif //FULL_HOUSE_LOGGER_H
