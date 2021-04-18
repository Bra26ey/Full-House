//
// Created by aleksandr on 15.04.2021.
//

#include "gtest/gtest.h"

#include "Logger.h"
#include "FileHandler.h"


TEST(FileHandler, read) {
    FileHandler log("input.txt", "output.txt");
    ASSERT_NO_THROW(log.Read());
}

TEST(FileHandler, read_write) {
    FileHandler writelog("input.txt", "output.txt");
    writelog.buffer << "{3, 4, 6, 200, 400, {1, 2, 3, 4, 6, 8}};";
    ASSERT_NO_THROW(writelog.Write());
}






