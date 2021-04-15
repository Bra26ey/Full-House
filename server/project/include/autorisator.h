#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <queue>

#include "user.h"
#include "definitions.h"

namespace network {

class Autorisator {
 public:
    explicit Autorisator(io_context &context);
    Autorisator(Autorisator &other) = delete;
    ~Autorisator();

    void Start();

 private:
    io_context &context_;
    user_queue &waitng_autorisation_;
    user_queue &autorised_;

 private:
    void DoAutorise();
};

}  // namespace network
