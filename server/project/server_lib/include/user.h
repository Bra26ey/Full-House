#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class User {
 public:
    explicit User(io_context &context) : context_(context),
                                         socket_(context),
                                         read_buffer(),
                                         write_buffer(),
                                         in(&read_buffer),
                                         out(&write_buffer),
                                         is_autorised(false) {};
   //  User(User &other) = delete;
    ~User();

    int Disconnect();
    bool IsConnected();

 public:
    io_context &context_;
    tcp::socket socket_;

    boost::asio::streambuf read_buffer;
    boost::asio::streambuf write_buffer;

    std::istream in;
    std::ostream out;

    std::string name_;
    bool is_autorised;
};

}  // namespace network
