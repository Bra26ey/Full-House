#include <iostream>

#include "base_database.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    BaseDB db;

    if (db.Connect("mysql://'localhost' dbname='test' user='root' password='*******'")) {
        std::cout << "Connection OK";
    }

    return 0;
}
