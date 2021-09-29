//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <vector>

#include "Logger.h"
#include "Card.h"
#include "PlayerInterface.h"
#include "Profile.h"
#include "TableBoard.h"

namespace logic {

struct Player: public PlayerInterface, public Profile {
    Player();
    int HandValue(std::vector<Card>& board_cards);  // TODO: Make one vector from player cards and board cards, Sort() it and analyze
    void Fold() override;
    int Call(int to_call) override;
    void Check() override;
    int Raise(int raised_money, int reraise) override;


    std::vector<Card> cards;
    bool in_pot;
    int current_stage_money_in_pot;

    unsigned int position;
private:

    static int HighCard(std::vector<Card>& combination_detect);

    static int CountComb(std::vector<Card>& combination_detect, int how_many_comb, int ammount_of_cards);  // (2, 2) == 2 pairs; (1, 3) == set; etc.

    static int OnePair(std::vector<Card>& combination_detect);
    static int TwoPairs(std::vector<Card>& combination_detect);
    static int Set(std::vector<Card>& combination_detect);

    static int Straight(std::vector<Card>& combination_detect);
    static int Flush(std::vector<Card>& combination_detect);

    static int FullHouse(std::vector<Card>& combination_detect);
    static int Quads(std::vector<Card>& combination_detect);

    static int StraightFlush(std::vector<Card>& combination_detect);
    static int RoyalFlush(std::vector<Card>& combination_detect);

    void Sort();
};

}  // namespace logic
