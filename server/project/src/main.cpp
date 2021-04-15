#include <iostream>

#include "server.h"
#include "utils.h"


int main(int argc, const char *argv[]) {
    if (unlikely(argc != 2)) {
        fprintf(stderr,"error: arg != 2\n");
        return -1;
    }

    network::Server server;
    server.Start();
    return 0;
}
