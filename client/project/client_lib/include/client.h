#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class Client {
 public:
    Client() : socket_(context_) {};
    Client(Client &other) = delete;
    ~Client();

    bool Connect();
    bool Disconnect();
    bool IsConnected();

    int Send(boost::asio::streambuf &buffer);
    int Read(boost::asio::streambuf &buffer);

   //  void Autorise(std::string const &login, std::string const &password);
   //  void Logout();

   //  void CreateRoom(std::string const &password);
   //  void JoinRoom(uint64_t const &id, std::string const &password);
   //  void LeaveRoom();

   //  void GetGameStatus();

   //  void GameRaise();
   //  void GameCold();
   //  void GameFlow();
   //  void GameBet(uint64_t const &sum);
   //  void GameSkip();

 private:
    io_context context_;
    tcp::socket socket_;

   //  TSQueue<std::string> msg_queue;
};

}  // namespace network
