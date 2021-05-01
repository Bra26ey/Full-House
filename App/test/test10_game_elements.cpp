#include "gtest/gtest.h"

#include "otherplayer.h"
#include "player.h"

#include "chips.h"
#include "card.h"
#include "playtable.h"
#include "playerinfolabel.h"

#include <QApplication>


TEST(otherplayer, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto player = new OtherPlayer(1));
    auto player = new OtherPlayer(1);
    ASSERT_NO_THROW(player->DisplayInfo());
    ASSERT_NO_THROW(player->show());

    delete player;
}


TEST(player, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto player = new Player(1);

    ASSERT_NO_THROW(player->DisplayInfo());
    ASSERT_NO_THROW(player->DisplayCards());
    ASSERT_NO_THROW(player->show());

    delete player;
}


TEST(chips, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto chip = new Chips(50));
    auto chip = new Chips(50);
    ASSERT_NO_THROW(chip->show());
    delete chip;
}


TEST(cards, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    int some_hardcoded_error_code_for_now = -100;
    ASSERT_ANY_THROW(auto card = new Card(0, 0));
    auto card = new Card(0, 0);
    ASSERT_NE(card->GetSuit(), some_hardcoded_error_code_for_now);
    ASSERT_NE(card->GetValue(), some_hardcoded_error_code_for_now);

    ASSERT_NO_THROW(card->show());

    delete card;
}


TEST(playtable, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto playtable = new PlayTable);
    auto playtable = new PlayTable;
    ASSERT_NO_THROW(playtable->show());
    delete playtable;
}

TEST(playerinfo, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto info = new PlayerInfoLabel);
    auto info = new PlayerInfoLabel;
    ASSERT_NO_THROW(info->show());
    delete info;
}
