#pragma once

#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic/atomic.hpp>

#include "tsqueue.h"

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class Client {
 public:
    Client() : socket_(context_), out_(&write_buffer_), in_(&read_buffer_), is_closeing(false) {};
    Client(Client &other) = delete;
    ~Client();

    std::string GetLastMsg();

    void Run();

    bool Connect();
    bool Disconnect();
    bool IsConnected();

    void Autorise(std::string const &login, std::string const &password);
    void Registrate(std::string const &login, std::string const &password);
    void Logout();

    void CreateRoom(std::string const &password);
    void JoinRoom(uint64_t const &id, std::string const &password);
    void LeaveRoom();

    void GetGameStatus();
    void StartGame();
    void GameRaise(uint64_t const &sum);
    void GameCall();
    void GameFold();
    void GameCheck();

    void AddMoney(uint64_t const &sum);
    void GetMoneyInfo();

 private:
    io_context context_;
    tcp::socket socket_;

    boost::asio::streambuf write_buffer_;
    boost::asio::streambuf read_buffer_;

    std::ostream out_;
    std::istream in_;

    TSQueue<std::string> msg_queue_;
    TSQueue<std::string> answers_queue_;

    boost::posix_time::ptime last_ping;

    boost::atomic<bool> is_closeing;
};

}  // namespace network
