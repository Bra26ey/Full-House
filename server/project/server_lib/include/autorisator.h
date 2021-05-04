#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>

#include "user.h"
#include "definitions.h"
#include "userbase.h"

namespace network {

class Autorisator : public boost::asio::coroutine {
 public:
    Autorisator(io_context &context, Userbase &userbase) : context_(context),
                                                           waitng_autorisation_(userbase.waitng_autorisation),
                                                           autorised_(userbase.autorised) {};
    Autorisator(Autorisator &other) = delete;
    ~Autorisator();

    void Start();

 private:
    io_context &context_;
    user_queue &waitng_autorisation_;
    user_queue &autorised_;

 private:
    void DoAutorise();
    void HandleAutorisation(std::shared_ptr<User> &user);
};

}  // namespace network
