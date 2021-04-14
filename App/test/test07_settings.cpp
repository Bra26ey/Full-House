#include "gtest/gtest.h"

#include "settingsfragment.h"
#include "mainwindow.h"

#include <QApplication>

TEST(Settings, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto set = new SettingsFragment);
    auto set = new SettingsFragment;
    ASSERT_NO_THROW(set->MutePressed());
    ASSERT_NO_THROW(set->ChangedSlider());
    ASSERT_NO_THROW(set->onMenuPressed());
    delete set;
}

TEST(Settings, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto set = new SettingsFragment;
    ASSERT_NO_THROW(set->onPause());
    ASSERT_NO_THROW(set->back());
    ASSERT_NO_THROW(set->onResume());
    delete set;
}

TEST(Settings, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto set = new SettingsFragment;
    ASSERT_NO_THROW(set->update());
    delete set;
}
