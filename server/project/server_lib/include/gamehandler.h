#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>

#include "gameroom.h"
#include "definitions.h"
#include "userbase.h"

namespace network {

typedef TSQueue<std::shared_ptr<GameRoom>> game_queue;

class GameHandler : public boost::asio::coroutine {
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
    game_queue waiting_start_;

    std::vector<std::shared_ptr<GameRoom>> gamerooms;

    user_queue &autorised_;
    user_queue &creating_game_;
    user_queue &accepting_game_;

 private:
    void HandlePlayers();
    void JoinPlayer(std::shared_ptr<User> user);
    void CreateGame(std::shared_ptr<User> user);
    void HandleRequest(std::shared_ptr<User> user);
    void HandleError(std::shared_ptr<User> user);
    void DoGame(std::shared_ptr<GameRoom> gameroom);
};

}  // namespace network
