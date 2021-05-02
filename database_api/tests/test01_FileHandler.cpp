#include "gtest/gtest.h"
#include "../FileHandler/FileHandler.h"


TEST(FileHandler, parse__ok) {
    FileHandler fh;
    auto parser = fh.Parse("config.txt");
    ASSERT_EQ(parser["status"], "OK");
}

TEST(FileHandler, parse__file_not_opened) {
    FileHandler fh;
    auto parser = fh.Parse("con.txt");
    ASSERT_EQ(parser["status"], FILE_NOT_OPENED_MSG);
}

TEST(FileHandler, parse_config__ok) {
    FileHandler fh;
    auto config = fh.ParseConfig();
    ASSERT_EQ(config.status_code, OK);
}

TEST(FileHandler, parse_config__file_not_opened) {
    FileHandler fh;
    auto config = fh.ParseConfig("error");
    ASSERT_EQ(config.status_code, FILE_NOT_OPENED);
}

TEST(FileHandler, parse_schemas__ok) {
    FileHandler fh;
    auto config = fh.ParseDbSchema();
    ASSERT_EQ(config.status_code, OK);
}

TEST(FileHandler, parse_schemas__file_not_opened) {
    FileHandler fh;
    auto schemas = fh.ParseDbSchema("error");
    ASSERT_EQ(schemas.status_code, FILE_NOT_OPENED);
}