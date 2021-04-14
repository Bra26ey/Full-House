#include "gtest/gtest.h"

#include "menufragment.h"
#include "mainwindow.h"

#include <QApplication>

TEST(Menu, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto menu = new MenuFragment);
    auto menu = new MenuFragment;
    ASSERT_NO_THROW(menu->onLoginPressed());
    ASSERT_NO_THROW(menu->onSearchPressed());
    ASSERT_NO_THROW(menu->onSettingsPressed());
    delete menu;
}

TEST(Menu, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto menu = new MenuFragment;
    ASSERT_NO_THROW(menu->onPause());
    ASSERT_NO_THROW(menu->back());
    ASSERT_NO_THROW(menu->onResume());
    delete menu;
}

TEST(Menu, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto menu = new MenuFragment;
    ASSERT_NO_THROW(menu->update());
    delete menu;
}
