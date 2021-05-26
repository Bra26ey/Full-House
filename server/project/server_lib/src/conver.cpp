#include "convert.h"

#include <boost/log/trivial.hpp>

logic::HandConfiguration convert(database::active_board const &board) {
    logic::HandConfiguration hand_config;

    hand_config.button_pos = board.hand_config.button_pos;
    hand_config.small_blind_pos = board.hand_config.small_blind_pos;
    hand_config.big_blind_pos = board.hand_config.big_blind_pos;
    hand_config.small_blind_bet = board.hand_config.small_blind_bet;
    hand_config.big_blind_bet = board.hand_config.big_blind_bet;
    hand_config.max_size_of_players = board.hand_config.max_size_of_players;
    hand_config.count_of_player_cards = board.hand_config.count_of_player_cards;

    for (auto &it : board.players) {
        auto player = std::make_shared<logic::Player>();
        player->id = it.id;
        player->name = it.username;
        player->money = it.reserved_money;
        player->position = it.position;
        hand_config.players.push_back(player);
    }

    return hand_config;
}

database::hand_configuration convert(logic::HandConfiguration const &hand_config) {
    database::hand_configuration board;

    board.button_pos = hand_config.button_pos;
    board.small_blind_pos = hand_config.small_blind_pos;
    board.big_blind_pos = hand_config.big_blind_pos;
    board.small_blind_bet = hand_config.small_blind_bet;
    board.big_blind_bet = hand_config.big_blind_bet;
    board.max_size_of_players = hand_config.max_size_of_players; 
    board.count_of_player_cards = hand_config.count_of_player_cards;

    return board;
}
