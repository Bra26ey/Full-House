#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "definitions.h"
#include "acceptor.h"
#include "autorisator.h"
#include "handler.h"
#include "gamehandler.h"

namespace network {

class Server {
 public:
    Server();
    Server(Server &other) = delete;
    ~Server();
    void Start();

 private:
    io_context context_;

    Acceptor acceptor_;
    Autorisator autorisator_;
    Handler handler_;
    GameHandler gamehandler_;

    user_queue waitng_autorisation_;
    user_queue autorised_;
    user_queue creating_game_;
    user_queue accepting_game_;
};

}  // namespace network
