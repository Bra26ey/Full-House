#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "definitions.h"
#include "userbase.h"
#include "acceptor.h"
#include "autorisator.h"
#include "handler.h"
#include "gamehandler.h"

namespace network {

class Server {
 public:
    Server() : acceptor_(context_, userbase_),
               autorisator_(context_, userbase_),
               handler_(context_, userbase_),
               gamehandler_(context_, userbase_) {};
    Server(Server &other) = delete;
    ~Server();
    void Start();

 private:
    io_context context_;

    Userbase userbase_;

    Acceptor acceptor_;
    Autorisator autorisator_;
    Handler handler_;
    GameHandler gamehandler_;
};

}  // namespace network
