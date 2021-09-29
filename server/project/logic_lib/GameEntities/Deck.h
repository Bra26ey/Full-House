//
// Created by aleksandr on 17.04.2021.
//

#pragma once

#include <vector>
#include "Card.h"

namespace logic {

class Deck {
public:
    Deck(size_t size);

    void Shuffle();
    Card& Peak();
    void Erase();
    void Init();

private:
    std::vector<Card> deck;
    size_t size = 52;
};

}  // namespace logic
