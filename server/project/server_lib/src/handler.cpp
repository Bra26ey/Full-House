#include "handler.h"

#include <boost/asio/yield.hpp>

using boost::asio::async_read;
using boost::asio::async_write;

namespace network {

Handler::~Handler() {}

void Handler::Start() {
    DoHandle();
}

void Handler::DoHandle() {
    // if (!autorised_.IsEmpty()) {
    //     auto user = autorised_.Pop();
    //     boost::asio::post(context_, boost::bind(&Autorisator::HandleAutorisation, this, user));
    //     BOOST_LOG_TRIVIAL(info) << "try autorisation";
    // }
    // boost::asio::post(context_, boost::bind(&Handler::DoHandle, this));
}

void Handler::HandleShareing(std::shared_ptr<User> user) {
    // reenter(this) {
    //     yield async_read_until(context_, user->socket_, user->read_buffer, '}', boost::bind(&Handler::HandleShareing, this, user));
    //     yield {

    //     }
    // }
}

}  // namespace network
