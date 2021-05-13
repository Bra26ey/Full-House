//
// Created by aleksandr on 15.04.2021.
//

#include "HandProcess.h"
#include "ConfigurationHandler.h"
#include "FileHandler.h"
#include "Card.h"

#include "Deck.h"

#define DECK_SIZE 52

void active_players(HandConfiguration& hand_config, int& count_of_players_in_pot) {
    count_of_players_in_pot = 0;
    for (auto it = hand_config.players.cbegin(); it != hand_config.players.cend(); ++it) {
        if (it->get()->in_pot) {
            ++count_of_players_in_pot;
        }
    }
}

int main() {
    Board board;
    board.pot = 0;
    board.cards.clear();

    Logger logger;

    Deck deck(DECK_SIZE);
    deck.Shuffle();

    FileHandler file_handler("input.txt");
    ConfigurationHandler config_handler(file_handler);
    config_handler.Read();
    HandConfiguration hand_config;
    config_handler.HandConfigurationInit(hand_config);

    HandProcess::DealCards(hand_config, deck, logger);
    int count_of_players_in_pot = hand_config.players.size();

    HandProcess::Preflop(hand_config, deck, board, logger);
    active_players(hand_config, count_of_players_in_pot);
    if (count_of_players_in_pot == 1) {
        return 0;
    }

    HandProcess::Flop(hand_config, deck, board, logger);
    active_players(hand_config, count_of_players_in_pot);
    if (count_of_players_in_pot == 1) {
        return 0;
    }

    HandProcess::Turn(hand_config, deck, board, logger);
    active_players(hand_config, count_of_players_in_pot);
    if (count_of_players_in_pot == 1) {
        return 0;
    }

    HandProcess::River(hand_config, deck, board, logger);
    active_players(hand_config, count_of_players_in_pot);
    if (count_of_players_in_pot == 1) {
        return 0;
    }

    HandProcess::PotDistribution(hand_config, deck, board, logger);



    return 0;
}


/*std::vector<Card> deck;
    Board board_;
    HandConfiguration hand_config;

    std::shared_ptr<Logger> logger_;

    unsigned int pos_of_raiser_;*/
// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1