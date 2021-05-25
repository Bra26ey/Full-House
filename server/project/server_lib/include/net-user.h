#pragma once

#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace network {

class User {
 public:
    explicit User(io_context &in_context) : context(in_context),
                                            socket(context),
                                            read_buffer(),
                                            write_buffer(),
                                            in(&read_buffer),
                                            out(&write_buffer),
                                            is_autorised(false),
                                            id(__UINT64_MAX__),
                                            room_id(__UINT64_MAX__) {};
   //  User(User &other) = delete;
    ~User() = default;

 public:
    io_context &context;
    tcp::socket socket;

    boost::asio::streambuf read_buffer;
    boost::asio::streambuf write_buffer;

    std::istream in;
    std::ostream out;

    boost::property_tree::ptree last_msg;

    bool is_autorised;
    boost::atomic<bool> is_talking;
    boost::atomic<bool> is_gaming;

    uint64_t id;
    uint64_t room_id;

    uint64_t money;
    std::string name;
};

}  // namespace network
