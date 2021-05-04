#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>

#include "definitions.h"
#include "userbase.h"

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::io_context;

constexpr uint16_t PORT = 2000;

namespace network {

class Acceptor : public boost::asio::coroutine {
 public:
    Acceptor(io_context &context, Userbase &userbase) : context_(context),
                                                        endpoint_(address::from_string("127.0.1.1"), PORT),
                                                        acceptor_(context_),
                                                        waitng_autorisation_(userbase.waitng_autorisation) {}
    ~Acceptor();

    void Start();

 private:
    io_context &context_;
    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;

    user_queue &waitng_autorisation_;

    void DoAccept(std::shared_ptr<User> &user);
};

}  // namespace network
