#include "gtest/gtest.h"

#include "gamesearchfragment.h"
#include "mainwindow.h"

#include <QApplication>

TEST(search, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto search = new GameSearchFragment);
    auto search = new GameSearchFragment;
    ASSERT_NO_THROW(search->CheckData());
    ASSERT_NO_THROW(search->onMenuPressed());
    ASSERT_NO_THROW(search->onMenuPressed());
    delete search;
}

TEST(search, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto search = new GameSearchFragment;
    ASSERT_NO_THROW(search->onPause());
    ASSERT_NO_THROW(search->onResume());
    ASSERT_NO_THROW(search->back());
    delete search;
}

TEST(search, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto search = new GameSearchFragment;
    ASSERT_NO_THROW(search->update());
    delete search;
}

