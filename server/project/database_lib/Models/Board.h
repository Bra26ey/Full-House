#pragma once
#ifndef FULL_HOUSE_BOARD_H
#define FULL_HOUSE_BOARD_H


#include "User.h"
#include <utility>
#include <tuple>

namespace database {

enum class Configuration {
    button_pos = 0,
    small_blind_pos = 0,
    big_blind_pos = 1,
    small_blind_bet = 1,
    big_blind_bet = 2,
    max_size_of_players = 6,
    count_of_player_cards = 2
};

typedef struct hand_configuration hand_configuration_t;
struct hand_configuration {
    unsigned int button_pos = static_cast<unsigned int>(Configuration::button_pos);
    unsigned int small_blind_pos = static_cast<unsigned int>(Configuration::small_blind_pos);
    unsigned int big_blind_pos = static_cast<unsigned int>(Configuration::big_blind_pos);

    int small_blind_bet = static_cast<int>(Configuration::small_blind_bet);
    int big_blind_bet = static_cast<int>(Configuration::big_blind_bet);

    unsigned int max_size_of_players = static_cast<unsigned int>(Configuration::max_size_of_players);
    unsigned int count_of_player_cards = static_cast<unsigned int>(Configuration::count_of_player_cards);

    hand_configuration() : button_pos(0), small_blind_pos(0), big_blind_pos(0), small_blind_bet(0),
                           big_blind_bet(0), max_size_of_players(0), count_of_player_cards(0) {}
    ~hand_configuration()=default;
};


typedef struct board board_t;
struct board {
    std::size_t board_id;
    std::size_t admin_id;
    std::string password;
    hand_configuration_t hand_config;
    std::size_t status_code;

    board() : board_id(0), admin_id(0), status_code(EMPTY_DATA) {}
    ~board()=default;
    board(const board&)=default;
    board& operator=(const board&)=default;
};


typedef struct player player_t;
struct player {
    std::size_t id;
    std::string username;
    std::string avatar;
    double reserved_money;
    int position;
};


typedef struct active_board active_board_t;
struct active_board {
    // structure store the active board:
    //      board_id:    the id of board
    //      players:     massive of player structure (id, username, money and position)
    //      hand_config: hand_configuration structure store blinds
    std::size_t board_id;
    std::vector<player_t> players;
    hand_configuration_t hand_config;
    std::size_t status_code;

    active_board() : board_id(0), status_code(EMPTY_DATA) {}
    ~active_board()=default;
    active_board(const active_board&)=default;
    active_board& operator=(const active_board&)=default;
};


class Board {
public:
    explicit Board()=default;
    ~Board()=default;
    Board(const Board&)=delete;
    Board& operator=(const Board&)=delete;

    // create board and return pair of board_id and status code
    std::pair<std::size_t, int> CreateBoard(const std::size_t&, const std::string&);
    int UpdateBoardAdmin(const std::size_t& board_id, const std::size_t& new_admin_id);
    // return board struct
    board_t GetBoard(const std::size_t&, bool);
    int DeleteBoard(const std::size_t&);
    int UpdateHandConfiguration(const std::size_t& board_id, const hand_configuration_t &hand_config);

    // return active board with players
    active_board_t GetActiveBoard(const std::size_t &);
    int SaveActiveBoard(const active_board_t&);
    int AddUserToBoard(const std::size_t&, const std::size_t&, const std::string&);
    int RemoveUserFromBoard(const std::size_t&, const std::size_t &);
    int SetReservedMoney(const std::size_t&, const std::size_t &, const double &);
    int UpdateUserPosition(const std::size_t& board_id, const std::size_t& player_id, const int& pos);

private:
    user_t GetUserId(const std::string&);
    int CheckPassword(const std::size_t&, const std::string&);

    template<class T>
    int UpdateField(const std::size_t& board_id, const std::size_t& player_id, const std::string &field_name,
                    const T& data, void (*functor)(std::shared_ptr<sql::PreparedStatement>&, const T&));

    SafetyConnection conn_;

};

} // namespace database


#endif //FULL_HOUSE_BOARD_H
