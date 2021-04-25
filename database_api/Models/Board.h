#pragma once
#ifndef UNTITLED_BOARD_H
#define UNTITLED_BOARD_H


#include "User.h"
#include <utility>


typedef struct board board_t;
struct board {
    // structure store the active board:
    //      table_id: the id of board
    //      users: massive of pair: (user's id, user's reserved money)
    std::size_t board_id;
    std::vector<std::pair<std::size_t, double>> users;
    std::size_t status_code;

    board() : board_id(0), status_code(EMPTY_DATA) {}
    ~board()=default;
    board(const board_t&)=default;
    board& operator=(const board_t&)=default;
};


class Board {
public:
    explicit Board(Connection* conn) : conn_(conn) {}
    ~Board() { conn_ = nullptr; }
    Board(const Board&)=delete;
    Board& operator=(const Board&)=delete;

    board_t GetBoard(const std::size_t &);
    std::pair<std::size_t, int> CreateBoard(const std::string&);
    int DeleteBoard(const std::size_t&);
    int AddUserToBoard(const std::size_t&, const std::string&);
    int RemoveUserFromBoard(const std::size_t&, const std::string&);
    int SetReservedMoney(const std::size_t&, const std::string&, const double &);

private:
    user_t GetUserId(const std::string&);

    Connection* conn_;

};


#endif //UNTITLED_BOARD_H
