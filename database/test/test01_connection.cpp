#include <gtest/gtest.h>

#include "../base_database.h"

TEST(Connect, testOK) {
    BaseDB db = BaseDB();
    EXPECT_EQ(db.Connect("mysql://'localhost' dbname='test' user='root' password='*******'"), true);
}

TEST(Connect, testERR) {
    BaseDB db = BaseDB();
    EXPECT_EQ(db.Connect("mysql://'localost' dbname='t' user='root' password=''"), false);
}
