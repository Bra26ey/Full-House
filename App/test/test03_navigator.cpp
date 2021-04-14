#include "gtest/gtest.h"


#include "navigator.h"
#include "screenfactory.h"
using namespace screens;

TEST(Navigator, test1) {
    QStackedWidget *container = nullptr;
    BaseScreensFactory *screensFactory = nullptr;
    auto nav = new FragmentNavigator(container, screensFactory);
    ASSERT_NO_THROW(nav->navigateTo(LOGIN_TAG));
    ASSERT_NO_THROW(nav->back());
    ASSERT_NO_THROW(nav->replace(LOGIN_TAG));
    ASSERT_NO_THROW(nav->newRootScreen(LOGIN_TAG));
    delete container;
    delete screensFactory;
}

TEST(Navigator, test2) {
}
