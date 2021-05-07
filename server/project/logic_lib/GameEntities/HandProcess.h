//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HANDPROCESS_H
#define FULL_HOUSE_HANDPROCESS_H

#include <list>
#include <memory>
#include <atomic>

#include "Player.h"
#include "Board.h"
#include "HandConfiguration.h"
#include "Logger.h"
#include "Deck.h"


constexpr int DECK_SIZE = 52;

class HandProcess {
public:
    void DealCards(HandConfiguration& hand_config, Deck& deck, Logger& logger);

    void Preflop(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: using GameStage
    void Flop(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: using GameStage
    void Turn(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: using GameStage
    void River(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: using GameStage
    void PotDistribution(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: using HandValue from Player and sort for all players to determine winner

    static std::list<std::shared_ptr<Player> >::iterator CircularNext(std::list<std::shared_ptr<Player> >& l, std::list<std::shared_ptr<Player> >::iterator it) {
        return std::next(it) == l.end() ? l.begin() : std::next(it);
    }

    static std::list<std::shared_ptr<Player> >::const_iterator CircularNext(std::list<std::shared_ptr<Player> >& l, std::list<std::shared_ptr<Player> >::const_iterator it) {
        return std::next(it) == l.cend() ? l.cbegin() : std::next(it);
    }

    static std::list<std::shared_ptr<Player> >::iterator CircularPrev(std::list<std::shared_ptr<Player> >& l, std::list<std::shared_ptr<Player> >::iterator it) {
        return std::prev(it) == l.begin() ? l.end() : std::prev(it);
    }

    static std::list<std::shared_ptr<Player> >::const_iterator CircularPrev(std::list<std::shared_ptr<Player> >& l, std::list<std::shared_ptr<Player> >::const_iterator it) {
        return std::prev(it) == l.cbegin() ? l.cend() : std::prev(it);
    }

    void GameStage(HandConfiguration& hand_config, Deck& deck, Board& board, Logger& logger);  // TODO: while loop pos_of_raiser == start_pos
    void GameLoop(bool& someone_raised, bool& first_round,
                         std::list<std::shared_ptr<Player> >::iterator& first_player,
                         std::list<std::shared_ptr<Player> >::iterator& position_of_raiser,
                         HandConfiguration& hand_config, Board& board, Logger& logger,
                         int& raised_money, int& players_in_pot, int& buf);

public:
    std::atomic<unsigned int> current_player_pos;

};



#endif //FULL_HOUSE_HANDPROCESS_H
