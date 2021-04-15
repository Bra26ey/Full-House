#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class User {
 public:
    explicit User(io_context &context);
    User(User &other) = delete;
    ~User();

    bool Connect();
    bool Disconnect();
    bool IsConnected();
    int Send(boost::asio::streambuf &buffer);
    int Read(boost::asio::streambuf &buffer);

 private:
    tcp::socket socket_;
    io_context &context_;
    std::string name;
};

}  // namespace network
