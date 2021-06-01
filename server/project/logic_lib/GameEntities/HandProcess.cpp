//
// Created by aleksandr on 15.04.2021.
//

#include <list>
#include <iostream>

#include "HandProcess.h"
#include "ConfigurationHandler.h"
#include "Card.h"
#include "Logger.h"
#include "Deck.h"

namespace logic {

bool HandProcess::one_player_in_pot(HandConfiguration &hand_config) {
    size_t count_of_players_in_pot = 0;
    for (auto it = hand_config.players.cbegin(); it != hand_config.players.cend(); ++it) {
        if (it->get()->in_pot) {
            ++count_of_players_in_pot;
        }
    }

    return count_of_players_in_pot == 1;
}


HandProcess::HandProcess(size_t ammount_of_cards) : current_player_pos(0), hand_config(),
                                                    logger(std::make_shared<Logger>()), deck_(ammount_of_cards),
                                                    board_(),
                                                    is_started_(false),
                                                    winner_pos_(NOT_DEFINDED) {}

void HandProcess::Init(HandConfiguration const &handconfiguration) {
    board_.cards.clear();
    board_.pot = 0;
    num_actions_ = 0;
    hand_config = handconfiguration;
    num_cards_on_table_ = 0;
    is_started_ = false;
    winner_pos_ = NOT_DEFINDED;
    hand_config.players.sort([](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) -> bool {
        return a.get()->position < b.get()->position;
    });
    // FileHandler file_handler(
    //         "/home/aleksandr/techn/Full-House/server/project/logic_lib/input.txt");
    // ConfigurationHandler config_handler(file_handler);
    // config_handler.Read();
    // config_handler.HandConfigurationInit(hand_config);
    need_next_stage = true;
    last_command_ = { NOT_DEFINDED, "", 0 };
    deck_.Init();
    deck_.Shuffle();
}

void HandProcess::DealCards() {
    mutex.lock();
    is_started_ = true;
    winner_pos_ = NOT_DEFINDED;
    for (auto it = hand_config.players.cbegin(); it != hand_config.players.cend(); ++it) {
        for (unsigned int i = 0; i < hand_config.count_of_player_cards; ++i) {
            it->get()->cards.push_back(deck_.Peak());
            deck_.Erase();
        }
    }
    logger->Log("Cards Deal");
}

bool HandProcess::Preflop() {

    logger->Log("Preflop Stage...");


    std::list<std::shared_ptr<Player> >::iterator position_of_big_blind;

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->in_pot = true;
        if (it->get()->position == hand_config.small_blind_pos) {
            it->get()->Call(hand_config.small_blind_bet);
        } else if (it->get()->position == hand_config.big_blind_pos) {
            it->get()->Call(hand_config.big_blind_bet);
            position_of_big_blind = it;
        }
    }

    int raised_money = hand_config.big_blind_bet;
    std::list<std::shared_ptr<Player> >::iterator position_of_raiser = position_of_big_blind;

    board_.pot += hand_config.small_blind_bet;
    board_.pot += hand_config.big_blind_bet;

    auto first_player = CircularNext(hand_config.players, position_of_big_blind);

    bool first_round = true;
    bool someone_raised = false;
    int players_in_pot = hand_config.players.size();
    int buf = 0;  // buffer in cases variable

    GameLoop(someone_raised, first_round,
                first_player, position_of_raiser,
                raised_money, players_in_pot, buf);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    if (one_player_in_pot(hand_config)) {
        need_next_stage = false;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        board_.cards.push_back(deck_.Peak());
        deck_.Erase();
    }
    for (auto it = board_.cards.cbegin(); it != board_.cards.cend(); ++it) {
        logger->Log("{} dropped on the board_", Card::ToString(it->suit, it->value));
    }
    num_cards_on_table_ = 3;
    logger->Log("Pot is {}", board_.pot);
    return true;
}

bool HandProcess::Flop() {
    if (!need_next_stage) {
        return false;
    }

    logger->Log("Flop stage...");

    GameStage();

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    if (one_player_in_pot(hand_config)) {
        need_next_stage = false;
        return false;
    }

    board_.cards.push_back(deck_.Peak());
    deck_.Erase();
    logger->Log("{} dropped on the board_ with the pot {}",
                Card::ToString((board_.cards.cend() - 1)->suit, (board_.cards.cend() - 1)->value), board_.pot);
    num_cards_on_table_ = 4;
    return true;
}

bool HandProcess::Turn() {
    if (!need_next_stage) {
        return false;
    }

    logger->Log("Turn stage...");

    GameStage();

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    if (one_player_in_pot(hand_config)) {
        need_next_stage = false;
        return false;
    }

    board_.cards.push_back(deck_.Peak());
    deck_.Erase();
    logger->Log("{} dropped on the board_ with the pot {}",
                Card::ToString((board_.cards.cend() - 1)->suit, (board_.cards.cend() - 1)->value), board_.pot);
    num_cards_on_table_ = 5;
    return true;
}

bool HandProcess::River() {
    if (!need_next_stage) {
        return false;
    }

    logger->Log("River stage...");
    logger->Log("board_ is...");
    for (auto it = board_.cards.cbegin(); it != board_.cards.cend(); ++it) {
        logger->Log("{}", Card::ToString(it->suit, it->value));
    }


    GameStage();

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    if (one_player_in_pot(hand_config)) {
        need_next_stage = false;
        return false;
    }

    return true;
}

void HandProcess::PotDistribution() {
    if (one_player_in_pot(hand_config)) {
        need_next_stage = false;
        auto it = std::find_if(hand_config.players.begin(), hand_config.players.end(),
                               [](std::shared_ptr<Player> &current) { return current->in_pot; });
        winner_pos_ = it->get()->position;
//        it->get()->money += board_.pot;
        board_.pot = 0;
        is_started_ = false;
        mutex.unlock();
        return;
    }

    logger->Log("Showdown...");

    int max_hand_value = 0;
    std::list<std::shared_ptr<Player> >::iterator max_hand_value_player;

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        if (!it->get()->in_pot) {
            continue;
        }
        int buf = it->get()->HandValue(board_.cards);
        if (buf > max_hand_value) {
            max_hand_value = buf;
            max_hand_value_player = it;
        }
        it->get()->current_stage_money_in_pot = 0;
        it->get()->cards.clear();
        it->get()->in_pot = false;
    }

    max_hand_value_player->get()->money += board_.pot;
    logger->Log("Player [{}] won [{}]", max_hand_value_player->get()->name, board_.pot);
    board_.cards.clear();

    winner_pos_ = max_hand_value_player->get()->position;
    is_started_ = false;
    mutex.unlock();
}

void HandProcess::GameStage() {

    std::list<std::shared_ptr<Player> >::iterator first_player;

    int players_in_pot = 0;

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        if (it->get()->in_pot) {
            ++players_in_pot;
        }
        if (it->get()->position == hand_config.small_blind_pos) {
            first_player = it;
        }
    }

    int raised_money = 0;
    std::list<std::shared_ptr<Player> >::iterator position_of_raiser;


    bool first_round = true;
    bool someone_raised = false;

    int buf = 0;  // buffer in cases variable

    GameLoop(someone_raised, first_round,
                first_player, position_of_raiser,
                raised_money, players_in_pot, buf);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

}

void HandProcess::GameLoop(bool &someone_raised, bool &first_round,
                            std::list<std::shared_ptr<Player> >::iterator &first_player,
                            std::list<std::shared_ptr<Player> >::iterator &position_of_raiser,
                            int &raised_money, int &players_in_pot, int &buf) {

    while (someone_raised | first_round) {
        someone_raised = false;
        auto it = first_player;
        for (size_t players = 0; players < hand_config.players.size(); it = CircularNext(hand_config.players, it), ++players) {
            if (one_player_in_pot(hand_config)) {
                need_next_stage = false;
                break;
            }

            if (!it->get()->in_pot || (it->get()->current_stage_money_in_pot == raised_money && !first_round) ||
                it->get()->money == 0) {
                continue;
            }

            std::cout << "\nYou " << "[" << it->get()->name << "] " << "have: "
                        << Card::ToString(it->get()->cards[0].suit, it->get()->cards[0].value)
                        << " and " << Card::ToString(it->get()->cards[1].suit, it->get()->cards[1].value)
                        << std::endl;

            uint8_t signal;
            if (it->get()->current_stage_money_in_pot == raised_money) {
                std::cout << "What to do: Check(3), Raise(2)" << std::endl;
                check_avaiable_ = true;
            } else {
                std::cout << "What to do: Fold(0), Call(1), Raise(2)" << std::endl;
                check_avaiable_ = false;
            }

            ++num_actions_;
            current_player_pos = it->get()->position;
            PlayerInfo action;
            mutex.unlock();
            do {
                action = command_queue.pop();  // block-function
            } while (current_player_pos != action.pos);
            mutex.lock();

            signal = command_[action.command];
            last_command_ = action;

            switch (signal) {
                case (FOLD_SIGNAL):
                    it->get()->Fold();
                    it->get()->in_pot = false;
                    players_in_pot -= 1;

                    logger->Log("[{}] folded\n\n", it->get()->name);
                    break;

                case (CALL_SIGNAL):
                    buf = it->get()->Call(raised_money - it->get()->current_stage_money_in_pot);  // pot money
                    board_.pot += it->get()->current_stage_money_in_pot - buf;

                    logger->Log("[{}] called from [{}] to [{}]\n\n", it->get()->name, buf,
                                it->get()->current_stage_money_in_pot);
                    break;

                case (RAISE_SIGNAL):
                    buf = it->get()->current_stage_money_in_pot;
                    raised_money = it->get()->Raise(raised_money, action.sum);
                    board_.pot += raised_money - buf;

                    someone_raised = true;
                    position_of_raiser = it;

                    logger->Log("[{}] raised to [{}]\n\n", it->get()->name, raised_money);
                    break;

                case (CHECK_SIGNAL):
                    it->get()->Check();
                    logger->Log("{} checked\n\n", it->get()->name);
                    break;
            }
        }

        if (players_in_pot == 1) {
            position_of_raiser->get()->money += board_.pot;
            logger->Log("{} won {} chips", position_of_raiser->get()->name, board_.pot);
            return;
        }

        first_round = false;
    }
}

boost::property_tree::ptree HandProcess::GetCardStatus(Card const &card) {
    boost::property_tree::ptree status;

    status.put("suit", static_cast<uint8_t>(card.suit));
    status.put("value", static_cast<uint8_t>(card.value));
    status.put("is-opend", card.show_everyone);

    return status;
}

boost::property_tree::ptree HandProcess::GetPlayerStatus(const std::shared_ptr<Player>& player) {
    boost::property_tree::ptree status;

    status.put("name", player->name);
    status.put("position", player->position);
    status.put("money", player->money);

    if (!is_started_) {
        return status;
    }

    status.put("in-pot", player->in_pot);

    boost::property_tree::ptree cards_status;
    for (auto &it : player->cards) {
        auto card = GetCardStatus(it);
        cards_status.push_back(std::make_pair("", card));
    }
    status.add_child("cards", cards_status);

    return status;
}

boost::property_tree::ptree HandProcess::GetGameStatus() {
    std::lock_guard<std::mutex> lock(mutex);
    boost::property_tree::ptree status;

    status.put("is-started", is_started_);
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
        players_status.push_back(std::make_pair("", player));
    }

    status.add_child("players", players_status);
    status.put("winner-position", winner_pos_);
    if (is_started_ || winner_pos_ != NOT_DEFINDED) {
        boost::property_tree::ptree last_command;
        last_command.put("position", last_command_.pos);
        last_command.put("command", last_command_.command);
        if (last_command_.command == "raise") {
            last_command.put("sum", last_command_.sum);
        }
        status.add_child("last-action", last_command);

    }

    if (!is_started_) {
        return status;
    }

    status.put("num-actions", num_actions_);
    status.put("current-turn", current_player_pos.load());
    status.put("current-actions", check_avaiable_ ? "raise-check" : "fold-call-raise");

    status.put("bank", board_.pot);
    status.put("num-cards-on-table", num_cards_on_table_);

    if (num_cards_on_table_ == 0) {
        return status;
    }

    boost::property_tree::ptree board_cards;
    for (auto &it : board_.cards) {
        auto card = GetCardStatus(it);
        board_cards.push_back(std::make_pair("", card));
    }
    status.add_child("board-сards", board_cards);

    return status;
}

} // namespace logic
