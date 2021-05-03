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

    ASSERT_NO_THROW(auto player = new OtherPlayer(3, "te3st", 20));
    auto player = new OtherPlayer(3, "te3st", 20);
    ASSERT_NO_THROW(player->FlipCards());
    ASSERT_NO_THROW(player->DiscardCards());
    ASSERT_NO_THROW(player->setBet(50));
    ASSERT_NO_THROW(player->show());

    delete player;
}


TEST(player, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    auto player = Player("name", 500);
    player.GiveCards(2,2,3,4);
    ASSERT_NO_THROW(player.FlipCards());
    ASSERT_NO_THROW(player.setBet(303));
    ASSERT_NO_THROW(player.DiscardCards());
}


TEST(chips, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    ASSERT_NO_THROW(auto chip = new Chips);
    auto chip = new Chips;
    ASSERT_NO_THROW(chip->AddToBank(50));
    ASSERT_EQ(chip->GetBank(), 50);
    delete chip;
}


TEST(cards, test1) {
    int argc = 1;
    char *argv[0];
    QApplication a(argc, argv);

    int some_hardcoded_error_code_for_now = -100;
    ASSERT_NO_THROW(auto card = new Card(1, 1));
    auto card = new Card(1, 1);
    ASSERT_EQ(card->GetSuit(), 1);
    ASSERT_EQ(card->GetValue(), 1);

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

    ASSERT_NO_THROW(auto info = new PlayerInfoLabel("test", 1));
    auto info = new PlayerInfoLabel("test", 1);
    ASSERT_NO_THROW(info->show());
    delete info;
}
