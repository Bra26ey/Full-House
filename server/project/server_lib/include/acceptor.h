#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "definitions.h"
#include "userbase.h"

using boost::asio::ip::tcp;
using boost::asio::io_context;

constexpr uint16_t PORT = 2000;

namespace network {

class Acceptor {
 public:
    Acceptor(io_context &context, Userbase &userbase) : context_(context),
                                                        endpoint_(tcp::v4(), PORT),
                                                        acceptor_(context_, endpoint_),
                                                        waitng_autorisation_(userbase.waitng_autorisation) {}
    ~Acceptor();

    void Start();

 private:
    io_context &context_;
    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;

    user_queue &waitng_autorisation_;

    void DoAccept();
};

}  // namespace network
