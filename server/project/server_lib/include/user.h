#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string.h>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class User {
 public:
    explicit User(io_context &context) : context_(context), socket_(context) {};
   //  User(User &other) = delete;
    ~User();

    bool Connect();
    bool Disconnect();
    bool IsConnected();
    int Send(boost::asio::streambuf &buffer);
    int Read(boost::asio::streambuf &buffer);

 private:
    io_context &context_;
    tcp::socket socket_;
    std::string name;
};

}  // namespace network
