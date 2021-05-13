//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_HANDPROCESS_H
#define FULL_HOUSE_HANDPROCESS_H

#include <list>
#include <memory>
#include <map>
#include <ConfigurationHandler.h>

#include <boost/property_tree/ptree.hpp>

#include "Player.h"
#include "TableBoard.h"
#include "HandConfiguration.h"
#include "Logger.h"
#include "Deck.h"
#include "SafeQueue.h"

constexpr uint8_t FOLD_SIGNAL = 0;
constexpr uint8_t CALL_SIGNAL = 1;
constexpr uint8_t RAISE_SIGNAL = 2;
constexpr uint8_t CHECK_SIGNAL = 3;

constexpr uint8_t DECK_SIZE = 52;

class HandProcess {
public:
    explicit HandProcess(size_t ammount_of_cards);
    void Init();
    void DealCards();

    bool Preflop();  // TODO: using GameStage
    bool Flop();  // TODO: using GameStage
    bool Turn();  // TODO: using GameStage
    bool River();  // TODO: using GameStage
    void PotDistribution();  // TODO: using HandValue from Player and sort for all players to determine winner

    boost::property_tree::ptree GetGameStatus();

    std::atomic<unsigned int> current_player_pos;
    HandConfiguration hand_config;
    std::shared_ptr<Logger> logger;

    // std::stringstream ss;
    SafeQueue<std::string> command_queue;

private:
    Deck deck_;
    Board board_;
    std::map<std::string, uint8_t> command_ {
        {"fold", FOLD_SIGNAL},
        {"call", CALL_SIGNAL},
        {"raise", RAISE_SIGNAL},
        {"check", CHECK_SIGNAL}
    };

    bool need_next_stage;

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

    static bool one_player_in_pot(HandConfiguration& hand_config);

    void GameStage();  // TODO: while loop pos_of_raiser == start_pos
    void GameLoop(bool& someone_raised, bool& first_round,
                  std::list<std::shared_ptr<Player> >::iterator& first_player,
                  std::list<std::shared_ptr<Player> >::iterator& position_of_raiser,
                  int& raised_money, int& players_in_pot, int& buf);

};



#endif //FULL_HOUSE_HANDPROCESS_H
