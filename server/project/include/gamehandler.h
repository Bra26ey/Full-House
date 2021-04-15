#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "gameroom.h"

namespace network {

class GameHandler {
 public:
    explicit GameHandler(io_context &context, user_queue &autorised, user_queue &creating_game, user_queue &accepting_game);
    GameHandler(GameHandler &other) = delete;
    ~GameHandler();

    void Start();

 private:
    io_context &context_;
    std::vector<GameRoom> gameroom;

    user_queue &autorised_;
    user_queue &creating_game_;
    user_queue &accepting_game_;

 private:
    int CreateGames();
    void AcceptAll();
    void DoGame();
};

}  // namespace network
