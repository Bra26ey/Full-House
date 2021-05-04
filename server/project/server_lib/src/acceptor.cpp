#include "acceptor.h"

#include <boost/asio/yield.hpp>

namespace network {

Acceptor::~Acceptor() {}

void Acceptor::Start() {
    BOOST_LOG_TRIVIAL(info) << "start acceptor on ep = " << endpoint_;
    acceptor_.open(endpoint_.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint_);
    acceptor_.listen();
    auto user = std::make_shared<User>(context_);
    DoAccept(user);
}

void Acceptor::DoAccept(std::shared_ptr<User> &user) {
    reenter(this) {
        while (true) {
            yield {
                BOOST_LOG_TRIVIAL(info) << "waiting for acception";
                acceptor_.async_accept(user->socket_, boost::bind(&Acceptor::DoAccept, this, user));
            }
            yield {
                BOOST_LOG_TRIVIAL(info) << "user accepted";
                waitng_autorisation_.Push(user);
                BOOST_LOG_TRIVIAL(info) << "user gone to autorisator";
                auto new_user = std::make_shared<User>(context_);
                context_.post(boost::bind(&Acceptor::DoAccept, this, new_user));
            }
        }
    }
}

}  // namespace network