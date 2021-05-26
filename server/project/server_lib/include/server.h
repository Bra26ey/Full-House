#pragma once

#include <string_view>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "definitions.h"
#include "userbase.h"
#include "usertalker.h"
#include "gametalker.h"

#include "User.h"
#include "Board.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;


// constexpr std::string_view SERVER_IP = "127.0.1.0";  // for local game
constexpr std::string_view SERVER_IP = "0.0.0.0";  // for public game

constexpr uint16_t PORT = 5000;

namespace network {

class Server {
 public:
    Server();
    Server(Server &other) = delete;
    ~Server();
    void Start();

 private:
    void StartListen(size_t thread_count);
    void ListenThread();
    void CleanUserTalkers();
    void CleanGameTalkers();
    void StartAccepting();
    
    void CreateRooms();
    void JoinPlayers();

    void HandleAcception(std::shared_ptr<User> &user);
    void AcceptionDone(std::shared_ptr<User> user);
    void AcceptionFailed();

 private:
    io_context context_;

    std::vector<std::shared_ptr<UserTalker>> usertalkers_;
    std::mutex usertalkers_mutex_;

    std::vector<std::shared_ptr<GameTalker>> gametalkers_;
    std::mutex gametalkers_mutex_;

    Userbase userbase_;

    database::User user_db_;
    database::Board board_db_;

    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;

    boost::thread_group threads_;
};

}  // namespace network
