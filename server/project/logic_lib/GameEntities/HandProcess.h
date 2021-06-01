//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <list>
#include <memory>
#include <map>
#include <mutex>
#include <ConfigurationHandler.h>

#include <boost/property_tree/ptree.hpp>

#include "Player.h"
#include "TableBoard.h"
#include "HandConfiguration.h"
#include "Logger.h"
#include "Deck.h"
#include "SafeQueue.h"

namespace logic {

    constexpr uint8_t FOLD_SIGNAL = 0;
    constexpr uint8_t CALL_SIGNAL = 1;
    constexpr uint8_t RAISE_SIGNAL = 2;
    constexpr uint8_t CHECK_SIGNAL = 3;

    constexpr uint8_t NOT_DEFINDED = 10;

    constexpr uint8_t DECK_SIZE = 52;

    struct PlayerInfo {
        uint8_t pos;
        std::string command;
        int sum;
    };

    class HandProcess {
    public:
        explicit HandProcess(size_t ammount_of_cards);

        void Init(HandConfiguration const &handconfiguration);

        void DealCards();

        bool Preflop();

        bool Flop();

        bool Turn();

        bool River();

        void PotDistribution();

        boost::property_tree::ptree GetGameStatus();

        std::atomic<unsigned int> current_player_pos;
        HandConfiguration hand_config;
        std::shared_ptr<Logger> logger;

        SafeQueue<PlayerInfo> command_queue;

    private:
        Deck deck_;
        Board board_;
        std::map<std::string, uint8_t> command_{
            {"fold",  FOLD_SIGNAL},
            {"call",  CALL_SIGNAL},
            {"raise", RAISE_SIGNAL},
            {"check", CHECK_SIGNAL}
        };
        std::mutex mutex;

        bool need_next_stage;

        bool is_started_;
        short num_cards_on_table_;
        uint8_t winner_pos_;
        uint16_t num_actions_;
        bool check_avaiable_;
        PlayerInfo last_command_;

        static boost::property_tree::ptree GetCardStatus(const Card &card);

        boost::property_tree::ptree GetPlayerStatus(const std::shared_ptr<Player> &player);

        static std::list<std::shared_ptr<Player> >::iterator
        CircularNext(std::list<std::shared_ptr<Player> > &l, std::list<std::shared_ptr<Player> >::iterator it) {
            return std::next(it) == l.end() ? l.begin() : std::next(it);
        }

        static std::list<std::shared_ptr<Player> >::const_iterator
        CircularNext(std::list<std::shared_ptr<Player> > &l, std::list<std::shared_ptr<Player> >::const_iterator it) {
            return std::next(it) == l.cend() ? l.cbegin() : std::next(it);
        }

        static std::list<std::shared_ptr<Player> >::iterator
        CircularPrev(std::list<std::shared_ptr<Player> > &l, std::list<std::shared_ptr<Player> >::iterator it) {
            return std::prev(it) == l.begin() ? l.end() : std::prev(it);
        }

        static std::list<std::shared_ptr<Player> >::const_iterator
        CircularPrev(std::list<std::shared_ptr<Player> > &l, std::list<std::shared_ptr<Player> >::const_iterator it) {
            return std::prev(it) == l.cbegin() ? l.cend() : std::prev(it);
        }

        bool one_player_in_pot(HandConfiguration &hand_config);

        void GameStage();  // TODO: while loop pos_of_raiser == start_pos
        void GameLoop(bool &someone_raised, bool &first_round,
                      std::list<std::shared_ptr<Player> >::iterator &first_player,
                      std::list<std::shared_ptr<Player> >::iterator &position_of_raiser,
                      int &raised_money, int &players_in_pot, int &buf);

    };

}  // namespace logic
