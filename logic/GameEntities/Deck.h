//
// Created by aleksandr on 17.04.2021.
//

#ifndef FULL_HOUSE_DECK_H
#define FULL_HOUSE_DECK_H

#include <vector>
#include "Card.h"

class Deck {
public:
    Deck(int size);

    void Shuffle();
    Card& Peak();
    void Erase();

private:
    std::vector<Card> deck;
    int size = 52;
};

#endif //FULL_HOUSE_DECK_H
