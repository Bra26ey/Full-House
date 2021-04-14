#include "gtest/gtest.h"

#include "registrationfragment.h"
#include "mainwindow.h"

#include <QApplication>

TEST(Registration, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto reg = new RegistrationFragment);
    auto reg = new RegistrationFragment;

    ASSERT_NO_THROW(reg->checkData());
    ASSERT_NO_THROW(reg->onRegistrationPressed());
    delete reg;
}

TEST(Registration, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto reg = new RegistrationFragment;

    ASSERT_NO_THROW(reg->onPause());
    ASSERT_NO_THROW(reg->back());
    ASSERT_NO_THROW(reg->onResume());
    delete reg;
}

TEST(Registration, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto reg = new RegistrationFragment;

    ASSERT_NO_THROW(reg->update());
    delete reg;
}
