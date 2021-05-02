#pragma once
#ifndef FULL_HOUSE_BOARD_H
#define FULL_HOUSE_BOARD_H


#include "User.h"
#include <utility>


typedef struct active_board active_board_t;
struct active_board {
    // structure store the active board:
    //      table_id: the id of board
    //      users: massive of pair: (user's id, user's reserved money)
    std::size_t board_id;
    std::vector<std::pair<std::size_t, double>> users;
    std::size_t status_code;

    active_board() : board_id(0), status_code(EMPTY_DATA) {}
    ~active_board()=default;
    active_board(const active_board&)=default;
    active_board& operator=(const active_board&)=default;
};


typedef struct board board_t;
struct board {
    std::size_t board_id;
    std::size_t admin_id;
    std::string password;
    std::size_t status_code;

    board() : board_id(0), admin_id(0), status_code(EMPTY_DATA) {}
    ~board()=default;
    board(const board&)=default;
    board& operator=(const board&)=default;
};


class Board {
public:
    explicit Board(Connection* conn) : conn_(conn) {}
    ~Board() { conn_ = nullptr; }
    Board(const Board&)=delete;
    Board& operator=(const Board&)=delete;

    // create board and return pair of board_id and status code
    std::pair<std::size_t, int> CreateBoard(const std::size_t&, const std::string&);
    // return board struct
    board_t GetBoard(const std::size_t&, bool);
    int DeleteBoard(const std::size_t&);

    // return active board with players
    active_board_t GetActiveBoard(const std::size_t &);
    int AddUserToBoard(const std::size_t&, const std::size_t&, const std::string&);
    int RemoveUserFromBoard(const std::size_t&, const std::size_t &);
    int SetReservedMoney(const std::size_t&, const std::size_t &, const double &);

private:
    user_t GetUserId(const std::string&);
    int CheckPassword(const std::size_t&, const std::string&);

    Connection* conn_;

};


#endif //FULL_HOUSE_BOARD_H
