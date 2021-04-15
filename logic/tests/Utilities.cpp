//
// Created by aleksandr on 15.04.2021.
//

#include "gtest/gtest.h"

#include "Logger.h"
#include "FileLogger.h"


TEST(FileLogger, read) {
    FileLogger log;
    ASSERT_NO_THROW(log.Read());
}

TEST(FileLogger, read_write) {
    FileLogger writelog;
    writelog.config = {3, 4, 6, 200, 400, {1, 2, 3, 4, 6, 8}};
    ASSERT_NO_THROW(writelog.Write());
}






