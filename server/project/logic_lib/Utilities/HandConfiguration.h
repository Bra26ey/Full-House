//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <list>
#include <memory>

#include <boost/property_tree/ptree.hpp>

#include "Player.h"

namespace logic {



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

}  // namespace logic
