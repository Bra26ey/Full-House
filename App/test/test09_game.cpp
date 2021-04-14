#include "gtest/gtest.h"

#include "gamefragment.h"
#include "mainwindow.h"

#include <QApplication>

TEST(game, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto game = new GameFragment);
    auto game = new GameFragment;
    ASSERT_NO_THROW(game->ChangedSlider());

    ASSERT_NO_THROW(game->onBetPressed());
    ASSERT_NO_THROW(game->onCheckPressed());
    ASSERT_NO_THROW(game->onFoldPressed());
    ASSERT_NO_THROW(game->onRaisePressed());


    ASSERT_NO_THROW(game->onLeavePressed());
    ASSERT_NO_THROW(game->onSettingsPressed());

    delete game;
}


TEST(game, test2) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto game = new GameFragment;
    ASSERT_NO_THROW(game->onPause());
    ASSERT_NO_THROW(game->onResume());
    ASSERT_NO_THROW(game->back());

    delete game;
}


TEST(game, test3) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto game = new GameFragment;
    ASSERT_NO_THROW(game->update());
    delete game;
}
