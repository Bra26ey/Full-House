//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HANDCONFIGURATION_H
#define FULL_HOUSE_HANDCONFIGURATION_H

#include <vector>

struct HandConfiguration {
    unsigned int button_pos;
    unsigned int small_blind_pos;
    unsigned int big_blind_pos;

    int small_blind_bet;
    int big_blind_bet;

    std::vector<unsigned int> pos_of_players;
};

#endif //FULL_HOUSE_HANDCONFIGURATION_H
