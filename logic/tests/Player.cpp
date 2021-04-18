//
// Created by aleksandr on 15.04.2021.
//

#include "gtest/gtest.h"
#include "Player.h"

TEST(Player, fold) {
    Player player;
    ASSERT_NO_THROW(player.Fold());
}

TEST(Player, call) {
    Player player;
    ASSERT_NO_THROW(player.Call());
}

TEST(Player, raise) {
    Player player;
    ASSERT_NO_THROW(player.Raise());
}

TEST(Player, check) {
    Player player;
    ASSERT_NO_THROW(player.Check());
}

TEST(Player, hand_value) {
    Player player;
    ASSERT_NO_THROW(player.HandValue());
}

TEST(Player, action) {
    Player player;
    ASSERT_NO_THROW(player.Action());
}
