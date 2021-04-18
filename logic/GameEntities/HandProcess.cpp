//
// Created by aleksandr on 15.04.2021.
//

#include <list>
#include <iostream>
#include "HandProcess.h"
#include "Card.h"
#include "Logger.h"
#include "Deck.h"

#define FOLD_SIGNAL 0
#define CALL_SIGNAL 1
#define RAISE_SIGNAL 2
#define CHECK_SIGNAL 3


void HandProcess::DealCards(HandConfiguration& hand_config, Deck& deck, Logger& logger) {
    for (auto it = hand_config.players.cbegin(); it != hand_config.players.cend(); ++it) {
        for (int i = 0; i < hand_config.count_of_player_cards; ++i) {
            it->get()->cards.push_back(deck.Peak());
            deck.Erase();
        }
    }
    logger.Log("Cards Deal");
}

void HandProcess::Preflop(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {

    logger.Log("Preflop Stage...");

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

    board.pot += hand_config.small_blind_bet;
    board.pot += hand_config.big_blind_bet;

    auto first_player = CircularNext(hand_config.players, position_of_big_blind);

    bool first_round = true;
    bool someone_raised = false;
    int players_in_pot = hand_config.players.size();
    int buf = 0;  // buffer in cases variable

    GameLoop(someone_raised, first_round,
             first_player, position_of_raiser,
             hand_config, board, logger,
             raised_money, players_in_pot, buf);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }
    for (int i = 0; i < 3; ++i) {
        board.cards.push_back(deck.Peak());
        deck.Erase();
    }
    for (auto it = board.cards.cbegin(); it != board.cards.cend(); ++it) {
        logger.Log("{} dropped on the board", Card::ToString(it->suit, it->value));
    }
    logger.Log("Pot is {}", board.pot);
}








void HandProcess::Flop(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {

    logger.Log("Flop stage...");

    GameStage(hand_config, deck, board, logger);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    board.cards.push_back(deck.Peak());
    deck.Erase();
    logger.Log("{} dropped on the board with the pot {}", Card::ToString((board.cards.cend() - 1)->suit, (board.cards.cend() - 1)->value), board.pot);

}

void HandProcess::Turn(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {

    logger.Log("Turn stage...");

    GameStage(hand_config, deck, board, logger);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

    board.cards.push_back(deck.Peak());
    deck.Erase();
    logger.Log("{} dropped on the board with the pot {}", Card::ToString((board.cards.cend() - 1)->suit, (board.cards.cend() - 1)->value), board.pot);

}

void HandProcess::River(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {


    logger.Log("River stage...");
    logger.Log("Board is...");
    for (auto it = board.cards.cbegin(); it != board.cards.cend(); ++it) {
        logger.Log("{}", Card::ToString(it->suit, it->value));
    }


    GameStage(hand_config, deck, board, logger);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

}

void HandProcess::PotDistribution(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {
    logger.Log("Showdown...");

    int max_hand_value = 0;
    std::list<std::shared_ptr<Player> >::iterator max_hand_value_player;

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        int buf = it->get()->HandValue(board.cards);
        if (buf > max_hand_value) {
            max_hand_value = buf;
            max_hand_value_player = it;
        }
        it->get()->current_stage_money_in_pot = 0;
        it->get()->cards.clear();
        it->get()->in_pot = false;
    }
    max_hand_value_player->get()->money += board.pot;
    logger.Log("Player [{}] won [{}]", max_hand_value_player->get()->name, board.pot);
    board.pot = 0;
    board.cards.clear();


}

void HandProcess::GameStage(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger) {

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
             hand_config, board, logger,
             raised_money, players_in_pot, buf);

    for (auto it = hand_config.players.begin(); it != hand_config.players.end(); ++it) {
        it->get()->current_stage_money_in_pot = 0;
    }

}

void HandProcess::GameLoop(bool& someone_raised, bool& first_round,
                           std::list<std::shared_ptr<Player> >::iterator& first_player,
                           std::list<std::shared_ptr<Player> >::iterator& position_of_raiser,
                           HandConfiguration& hand_config, Board& board, Logger& logger,
                           int& raised_money, int& players_in_pot, int& buf) {

    while (someone_raised | first_round) {
        someone_raised = false;
        auto it = first_player;
        for (int players = 0; players < hand_config.players.size(); it = CircularNext(hand_config.players, it), ++players) {

            if (!it->get()->in_pot || (it->get()->current_stage_money_in_pot == raised_money && !first_round) || it->get()->money == 0) {
                continue;
            }

            std::cout << "You " << "[" << it->get()->name << "] " << "have: " << Card::ToString(it->get()->cards[0].suit, it->get()->cards[0].value)
                      << " and " << Card::ToString(it->get()->cards[1].suit, it->get()->cards[1].value) << std::endl;

            int signal;
            if (it->get()->current_stage_money_in_pot == raised_money) {
                std::cout << "What to do: Check(3), Raise(2)" << std::endl;
            } else {
                std::cout << "What to do: Fold(0), Call(1), Raise(2)" << std::endl;
            }
            std::cin >> signal;

            switch (signal) {
                case (FOLD_SIGNAL):
                    it->get()->Fold();
                    it->get()->in_pot = false;
                    players_in_pot -= 1;
                    logger.Log("[{}] folded\n\n", it->get()->name);
                    break;

                case (CALL_SIGNAL):
                    buf = it->get()->Call(raised_money - it->get()->current_stage_money_in_pot);  // returns money in pot
                    board.pot += it->get()->current_stage_money_in_pot - buf;
                    logger.Log("[{}] called from [{}] to [{}]\n\n", it->get()->name, buf, it->get()->current_stage_money_in_pot);
                    break;

                case (RAISE_SIGNAL):
                    try {
                        buf = it->get()->current_stage_money_in_pot;
                        raised_money = it->get()->Raise(raised_money);
                        board.pot += raised_money - buf;

                        someone_raised = true;
                        position_of_raiser = it;

                        logger.Log("[{}] raised to [{}]\n\n", it->get()->name, raised_money);
                    } catch (...) {
                        logger.Log("Bad Raise");
                    }
                    break;

                case (CHECK_SIGNAL):
                    it->get()->Check();
                    logger.Log("{} checked\n\n", it->get()->name);
                    break;
            }
        }

        if (players_in_pot == 1) {
            position_of_raiser->get()->money += board.pot;
            logger.Log("{} won {} chips", position_of_raiser->get()->name, board.pot);
            return;
        }

        first_round = false;
    }
}

// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1