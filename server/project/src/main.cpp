#include <iostream>

#include "server.h"
#include "utils.h"

int main(int argc, const char *argv[]) {
    if (unlikely(argc != 1)) {
        std::cout << "error: arg != 1" << std::endl; 
        return -1;
    }

    network::Server server;
    server.Start();
    return 0;
}
