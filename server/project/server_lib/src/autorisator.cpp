#include "autorisator.h"

#include <boost/asio/yield.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

static void Clean(std::istream &in) {
    if (in.eof()) {
        return;
    }

    char c;
    in >> c;
    while (c != '}') {
        in >> c;
    }
}

namespace network {

Autorisator::~Autorisator() {}

void Autorisator::Start() {
    BOOST_LOG_TRIVIAL(info) << "start autorise";
    boost::asio::post(context_, boost::bind(&Autorisator::DoAutorise, this));
}

void Autorisator::DoAutorise() {
    if (!waitng_autorisation_.IsEmpty()) {
        auto user = waitng_autorisation_.Pop();
        boost::asio::post(context_, boost::bind(&Autorisator::HandleAutorisation, this, user));
        BOOST_LOG_TRIVIAL(info) << "try autorisation";
    }
    boost::asio::post(context_, boost::bind(&Autorisator::DoAutorise, this));
}

void Autorisator::HandleAutorisation(std::shared_ptr<User> &user) {
    reenter(this) {
        while (true) {
            yield async_read_until(user->socket_, user->read_buffer, '}', boost::bind(&Autorisator::HandleAutorisation, this, user));
            yield {
                // TODO(ANDY) ask database for login/password valid
                std::string str;
                user->in >> str;
                user->is_autorised = (str == "{login:");
                if (user->is_autorised) {
                    user->in >> user->name_;
                    user->out << "{ok}";
                    BOOST_LOG_TRIVIAL(info) << "user is autorised. name: " << user->name_;
                } else {
                    user->out << "{not ok}";
                    BOOST_LOG_TRIVIAL(info) << "autorisation: invalid data";
                }
                Clean(user->in);
                boost::asio::post(context_, boost::bind(&Autorisator::HandleAutorisation, this, user));
            }
            yield async_write(user->socket_, user->write_buffer, boost::bind(&Autorisator::HandleAutorisation, this, user));
            yield {
                BOOST_LOG_TRIVIAL(info) << "waiting for new autorise";
                if (user->is_autorised) {
                    autorised_.Push(user);
                } else {
                    waitng_autorisation_.Push(user);
                }
            }
        }
    }
}

}  // namespace network
