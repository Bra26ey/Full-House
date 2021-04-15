//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_BOARD_H
#define FULL_HOUSE_BOARD_H

#include <vector>
#include "Card.h"


struct Board {
    int pot;
    std::vector<Card> cards;
};

#endif //FULL_HOUSE_BOARD_H
