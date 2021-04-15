//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_PLAYER_H
#define FULL_HOUSE_PLAYER_H

#include <vector>
#include "Card.h"
#include "PlayerInterface.h"

struct Player: public PlayerInterface {
    void HandValue();  // TODO: Make one vector from player cards and board cards, Sort() it and analyze
    void Fold() override;
    void Call() override;
    void Check() override;
    void Raise() override;

    void Action();  // fold or call or etc.

    std::vector<Card> cards;
    int cash;  // cash on table (less then money)
    bool in_pot;
private:
    void Sort();
};

#endif //FULL_HOUSE_PLAYER_H
