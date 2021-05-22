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

    std::list<std::shared_ptr<Player>> players;

    HandConfiguration& operator=(const HandConfiguration &other) {
        if (this == &other) {
            return *this;
        }

        button_pos = other.button_pos;
        small_blind_pos = other.small_blind_pos;
        big_blind_pos = other.big_blind_pos;

        small_blind_bet = other.small_blind_bet;
        big_blind_bet = other.big_blind_bet;

        max_size_of_players = other.max_size_of_players;
        count_of_player_cards = other.count_of_player_cards;
        players.clear();
        for (auto it : other.players) {
            players.push_back(it);
        }

        return *this;
    }
};

}  // namespace logic
