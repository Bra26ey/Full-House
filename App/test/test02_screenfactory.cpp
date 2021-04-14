#include "gtest/gtest.h"

#include "mainwindow.h"
#include "screenfactory.h"

#include "loginfragment.h"
#include "registrationfragment.h"
#include "menufragment.h"
#include "settingsfragment.h"
#include "gamesearchfragment.h"
#include "gamefragment.h"

#include <QLinkedList>
#include <QApplication>


TEST(ScreenFactory, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto factory = new ScreensFactory;
    QString expected = screens::LOGIN_TAG;
    ASSERT_STREQ(factory->createStart().toLatin1(), expected.toLatin1());
    delete factory;
}


TEST(ScreenFactory, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto factory = new ScreensFactory;
    QString str = "blabla";
    ASSERT_EQ(factory->create(str), nullptr);
    delete factory;
}


TEST(ScreenFactory, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);


    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "login";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}


TEST(ScreenFactory, test4) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "registration";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}

TEST(ScreenFactory, test5) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "menu";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}


TEST(ScreenFactory, test6) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "settings";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}

TEST(ScreenFactory, test7) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "search";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}

TEST(ScreenFactory, test8) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QLinkedList<BaseFragment*> stack;
    auto factory = new ScreensFactory;
    QString str = "game";
    stack.append(factory->create(str));
    ASSERT_NE(stack.first(), nullptr);
    delete factory;
}
