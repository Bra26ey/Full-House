#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "gameroom.h"

namespace network {

class GameHandler {
 public:
    GameHandler(io_context &context, Userbase &userbase) : context_(context),
                                                           autorised_(userbase.autorised),
                                                           creating_game_(userbase.creating_game),
                                                           accepting_game_(userbase.accepting_game) {}
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
