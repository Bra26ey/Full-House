#include "gtest/gtest.h"

#include "loginfragment.h"
#include "mainwindow.h"

#include <QApplication>


TEST(LoginFragment, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto login = new LoginFragment);
    auto login = new LoginFragment;
    ASSERT_NO_THROW(login->CheckData());
    ASSERT_NO_THROW(login->onRegistrationPressed());
    ASSERT_NO_THROW(login->onLoginPressed());
    delete login;
}

TEST(LoginFragment, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto login = new LoginFragment;
    ASSERT_NO_THROW(login->onResume());
    ASSERT_NO_THROW(login->back());
    ASSERT_NO_THROW(login->onPause());
    delete login;
}

TEST(LoginFragment, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto login = new LoginFragment;
    ASSERT_NO_THROW(login->update());
    delete login;
}
