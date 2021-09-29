//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <vector>
#include "Card.h"

namespace logic {

struct Board {
    int pot;
    std::vector<Card> cards;

    Board() {
        pot = 0;
        cards.clear();
    }
};

}  // namespace logic
