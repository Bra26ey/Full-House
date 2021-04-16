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

 private:
    io_context context_;
    tcp::socket socket_;
};

}  // namespace network
