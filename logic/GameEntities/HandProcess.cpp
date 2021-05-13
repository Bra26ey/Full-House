//
// Created by aleksandr on 15.04.2021.
//

#include <list>
#include <iostream>

#include <boost/asio.hpp>

#include "HandProcess.h"
#include "ConfigurationHandler.h"
#include "Card.h"
#include "Logger.h"
#include "Deck.h"

bool HandProcess::one_player_in_pot(HandConfiguration& hand_config) {
    return std::count_if(hand_config.players.begin(), hand_config.players.end(), [](auto it){
        return it->in_pot;
    }) == 1;
}


HandProcess::HandProcess(size_t ammount_of_cards):  current_player_pos(0),
                                                    hand_config(),
                                                    logger(std::make_shared<Logger>()),
                                                    deck_(ammount_of_cards),
                                                    board_() {}

void HandProcess::Init() {
    FileHandler file_handler("../input.txt");
    ConfigurationHandler config_handler(file_handler);
    config_handler.Read();
    config_handler.HandConfigurationInit(hand_config);
    need_next_stage = true;
    deck_.Init();
    deck_.Shuffle();
}

void HandProcess::DealCards() {
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
    logger->Log("{} dropped on the board_ with the pot {}", Card::ToString((board_.cards.cend() - 1)->suit, (board_.cards.cend() - 1)->value), board_.pot);

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
    logger->Log("{} dropped on the board_ with the pot {}", Card::ToString((board_.cards.cend() - 1)->suit, (board_.cards.cend() - 1)->value), board_.pot);

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
        return;
    }

    logger->Log("Showdown...");

    int max_hand_value = 0;
    std::list<std::shared_ptr<Player> >::iterator max_hand_value_player;

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
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
    board_.pot = 0;
    board_.cards.clear();


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

void HandProcess::GameLoop(bool& someone_raised, bool& first_round,
                           std::list<std::shared_ptr<Player> >::iterator& first_player,
                           std::list<std::shared_ptr<Player> >::iterator& position_of_raiser,
                           int& raised_money, int& players_in_pot, int& buf) {

    while (someone_raised | first_round) {
        someone_raised = false;
        auto it = first_player;
        for (size_t players = 0; players < hand_config.players.size(); it = CircularNext(hand_config.players, it), ++players) {
            if (one_player_in_pot(hand_config)) {
                need_next_stage = false;
                break;
            }

            current_player_pos = it->get()->position;

            if (!it->get()->in_pot || (it->get()->current_stage_money_in_pot == raised_money && !first_round) || it->get()->money == 0) {
                continue;
            }

            std::cout << "\nYou " << "[" << it->get()->name << "] " << "have: " << Card::ToString(it->get()->cards[0].suit, it->get()->cards[0].value)
                      << " and " << Card::ToString(it->get()->cards[1].suit, it->get()->cards[1].value) << std::endl;

            uint8_t signal;
            std::string action;
            if (it->get()->current_stage_money_in_pot == raised_money) {
                std::cout << "What to do: Check(3), Raise(2)" << std::endl;
            } else {
                std::cout << "What to do: Fold(0), Call(1), Raise(2)" << std::endl;
            }

            // boost::asio::streambuf buffer;
            // std::istream os(&buffer);
            // boost::asio::read_until(ss, buffer, '\n');
            action = command_queue.pop();  // block-function
            // os >> action;
            signal = command_[action];

            switch (signal) {
                case (FOLD_SIGNAL):
                    it->get()->Fold();
                    it->get()->in_pot = false;
                    players_in_pot -= 1;
                    logger->Log("[{}] folded\n\n", it->get()->name);
                    break;

                case (CALL_SIGNAL):
                    buf = it->get()->Call(raised_money - it->get()->current_stage_money_in_pot);  // returns money in pot
                    board_.pot += it->get()->current_stage_money_in_pot - buf;
                    logger->Log("[{}] called from [{}] to [{}]\n\n", it->get()->name, buf, it->get()->current_stage_money_in_pot);
                    break;

                case (RAISE_SIGNAL):
                    try {
                        buf = it->get()->current_stage_money_in_pot;
                        int reraise;
                        std::cout << "Enter reraise for " << raised_money << " :\t";
                        reraise = std::stoi(command_queue.pop()); 
                        raised_money = it->get()->Raise(raised_money, reraise);
                        board_.pot += raised_money - buf;

                        someone_raised = true;
                        position_of_raiser = it;

                        logger->Log("[{}] raised to [{}]\n\n", it->get()->name, raised_money);
                    } catch (...) {
                        logger->Log("Bad Raise");
                    }
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




// boost::property_tree::ptree GetCardStatus(Card &card) {
//     boost::property_tree::ptree status;

//     status.put("suit", card.Suit);
//     status.put("value", card.Value);
//     status.put("is-opend", card.show_everyone);

//     return status;
// }

// boost::property_tree::ptree GetPlayerStatus(std::shared_ptr<Player> &player) {
//     boost::property_tree::ptree status;

//     boost::property_tree::ptree cards_status;
//     for (auto &it : player->cards) {
//         auto card = GetCardStatus(it);
//         cards_status.put("", card);
//     }

//     status.add_child("cards", cards_status);

//     status.put("in-pot", player->in_pot);
//     status.put("current-stage-money-in-pot", player->current_stage_money_in_pot);
//     status.put("position", player->position);

//     return status;
// }

// boost::property_tree::ptree HandProcess::GetGameStatus() {
//     boost::property_tree::ptree status;

//     status.put("button-pos", hand_config.button_pos);
//     status.put("small-blind-pos", hand_config.small_blind_pos);
//     status.put("big-blind-pos", hand_config.big_blind_pos);

//     status.put("small-blind-bet", hand_config.small_blind_bet);
//     status.put("big-blind-bet", hand_config.big_blind_bet);

//     status.put("max-size-of-players", hand_config.max_size_of_players);
//     status.put("count-of-player-cards", hand_config.count_of_player_cards);

//     boost::property_tree::ptree players_status;
//     for (auto &it : hand_config.players) {
//         auto player = GetPlayerStatus(it);
//         players_status.put("", player);
//     }

//     status.add_child("players", players_status);

//     return status;
// }

// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1