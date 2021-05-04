#include "user.h"

namespace network {

User::~User() {
    Disconnect();
}

int User::Disconnect() {
    if (!IsConnected()) {
        return 0;
    }

    socket_.close();
    return 0;
}

bool User::IsConnected() {
    return socket_.is_open();
}

}  // namespace network
