#include "server.h"

#include <boost/log/trivial.hpp>

namespace network {

Server::~Server() {}

void Server::Start() {
    context_.post(boost::bind(&Acceptor::Start, &acceptor_));
    context_.post(boost::bind(&Autorisator::Start, &autorisator_));
    context_.post(boost::bind(&Handler::Start, &handler_));
    context_.post(boost::bind(&GameHandler::Start, &gamehandler_));

    const size_t thread_nom = boost::thread::hardware_concurrency();
    BOOST_LOG_TRIVIAL(info) << "threads avaiable: " << thread_nom;
    StartListen(thread_nom);
    threads_.join_all();
}

void Server::ListenThread() {
    context_.run();
}

void Server::StartListen(size_t thread_count) {
    BOOST_LOG_TRIVIAL(info) << "start server";
    for (size_t i = 0; i < thread_count; ++i) {
        threads_.create_thread(boost::bind(&Server::ListenThread, this));
    }
}

}