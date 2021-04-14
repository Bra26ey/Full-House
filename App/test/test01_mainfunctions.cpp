#include "gtest/gtest.h"

#include "mainwindow.h"

#include <QApplication>

TEST(main_function, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);
    MainWindow w;
    ASSERT_NO_THROW(w.show());
    ASSERT_EQ(a.exec(), 0);
    ASSERT_NO_THROW(a.quit());
}
