//
// Created by aleksandr on 15.04.2021.
//

#include "gtest/gtest.h"
#include "TableBoard.h"
#include "Card.h"
#include "HandProcess.h"


TEST(HandProcess, deal_cards) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.DealCards());
}

TEST(HandProcess, preflop) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.Preflop());
}

TEST(HandProcess, flop) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.Flop());
}

TEST(HandProcess, turn) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.Turn());
}

TEST(HandProcess, river) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.River());
}

TEST(HandProcess, pot_distribution) {
    HandProcess hand_process;
    ASSERT_NO_THROW(hand_process.PotDistribution());
}

/*TEST(Card, show_card) {
    Card card = {2, 11};
    card.Show();
    EXPECT_EQ(card.show_everyone, true);
}*/
