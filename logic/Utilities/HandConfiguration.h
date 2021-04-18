//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HANDCONFIGURATION_H
#define FULL_HOUSE_HANDCONFIGURATION_H

#include <list>
#include <memory>
#include "Player.h"

struct HandConfiguration {
    unsigned int button_pos;
    unsigned int small_blind_pos;
    unsigned int big_blind_pos;

    int small_blind_bet;
    int big_blind_bet;

    unsigned int max_size_of_players;
    unsigned int count_of_player_cards;

    std::list<std::shared_ptr<Player> > players;
};

#endif //FULL_HOUSE_HANDCONFIGURATION_H
