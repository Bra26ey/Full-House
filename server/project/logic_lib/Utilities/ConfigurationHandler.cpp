//
// Created by aleksandr on 17.04.2021.
//
#include <algorithm>

#include "LogicFileHandler.h"
#include "HandConfiguration.h"
#include "ConfigurationHandler.h"
#include "Player.h"

namespace logic {

ConfigurationHandler::ConfigurationHandler(FileHandler& file_handler): file_handler(file_handler) {}

void ConfigurationHandler::HandConfigurationInit(HandConfiguration& hand_config) {
    file_handler.buffer >> hand_config.button_pos;
    file_handler.buffer >> hand_config.small_blind_pos;
    file_handler.buffer >> hand_config.big_blind_pos;
    file_handler.buffer >> hand_config.small_blind_bet;
    file_handler.buffer >> hand_config.big_blind_bet;
    file_handler.buffer >> hand_config.max_size_of_players;
    file_handler.buffer >> hand_config.count_of_player_cards;
    Player buf_player;
    while (file_handler.buffer >> buf_player.id >> buf_player.name >> buf_player.money >> buf_player.position) {
        hand_config.players.push_back(std::make_shared<Player>(buf_player));
    }
    hand_config.players.sort([](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) -> bool {
        return a.get()->position < b.get()->position;
    });
}

void ConfigurationHandler::Read() {
    file_handler.Read();
}

void ConfigurationHandler::Write() {
    file_handler.Write();
}

}  // namespace logic
