#include "gtest/gtest.h"


#include "navigator.h"
#include "screenfactory.h"

#include <QApplication>
using namespace screens;

TEST(Navigator, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    QStackedWidget *container = new QStackedWidget;
    BaseScreensFactory *screensFactory = new ScreensFactory;
    auto nav = new FragmentNavigator(container, screensFactory);
    ASSERT_NO_THROW(nav->navigateTo(LOGIN_TAG));
    ASSERT_NO_THROW(nav->back());
    ASSERT_NO_THROW(nav->replace(LOGIN_TAG));
    ASSERT_NO_THROW(nav->newRootScreen(LOGIN_TAG));
    delete container;
    delete screensFactory;
}

