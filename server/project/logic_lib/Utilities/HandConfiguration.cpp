#include "HandConfiguration.h"

namespace logic {

static boost::property_tree::ptree GetCardStatus(Card const &card) {
    boost::property_tree::ptree status;

    status.put("suit", static_cast<uint8_t>(card.suit));
    status.put("value", static_cast<uint8_t>(card.value));
    status.put("is-opend", card.show_everyone);

    return status;
}

static boost::property_tree::ptree GetPlayerStatus(std::shared_ptr<Player> const &player) {
    boost::property_tree::ptree status;

    boost::property_tree::ptree cards_status;
    for (auto &it : player->cards) {
        auto card = GetCardStatus(it);
        cards_status.add_child("", card);
    }

    status.put("in-pot", player->in_pot);
    status.put("current-stage-money-in-pot", player->current_stage_money_in_pot);
    status.put("position", player->position);

    status.add_child("cards", cards_status);


    return status;
}

boost::property_tree::ptree GetGameStatus(HandConfiguration const &hand_config) {
    boost::property_tree::ptree status;

    status.put("button-pos", hand_config.button_pos);
    status.put("small-blind-pos", hand_config.small_blind_pos);
    status.put("big-blind-pos", hand_config.big_blind_pos);

    status.put("small-blind-bet", hand_config.small_blind_bet);
    status.put("big-blind-bet", hand_config.big_blind_bet);

    status.put("max-size-of-players", hand_config.max_size_of_players);
    status.put("count-of-player-cards", hand_config.count_of_player_cards);

    boost::property_tree::ptree players_status;
    for (auto &it : hand_config.players) {
        auto player = GetPlayerStatus(it);
        players_status.add_child("", player);
    }

    status.add_child("players", players_status);

    return status;
}

}  // namespace logic
