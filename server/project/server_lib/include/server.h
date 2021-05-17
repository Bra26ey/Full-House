#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "definitions.h"
#include "userbase.h"
#include "usertalker.h"
#include "gametalker.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

constexpr uint16_t PORT = 5000;

namespace network {

class Server {
 public:
    Server() : context_(),
               endpoint_(address::from_string("0.0.0.0"), PORT),
               acceptor_(context_) {};
    Server(Server &other) = delete;
    ~Server();
    void Start();

 private:
    void StartListen(size_t thread_count);
    void ListenThread();
    void CleanUserTalkers();
    void CleanGameTalkers();
    void StartAccepting();
    void HandleAcception(std::shared_ptr<User> &user);
    void CreateRooms();
    void JoinPlayers();
    

 private:
    io_context context_;

    std::vector<std::shared_ptr<UserTalker>> usertalkers_;
    std::mutex usertalkers_mutex_;

    std::vector<std::shared_ptr<GameTalker>> gametalkers_;
    std::mutex gametalkers_mutex_;

    Userbase userbase_;

    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;

    boost::thread_group threads_;
};

}  // namespace network
