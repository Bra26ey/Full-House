#ifndef FULL_HOUSE_POOLCONNECTIONS_H
#define FULL_HOUSE_POOLCONNECTIONS_H


#include <iostream>
#include <string>
#include <future>
#include <mutex>

#include "Connection.h"


class PoolConnections {
public:
    // prohibit copying the object
    PoolConnections(const PoolConnections&) = delete;
    PoolConnections& operator=(const PoolConnections&) = delete;

    static PoolConnections* GetInstance();

    int SetParams(const std::string& filename);
    Connection* GetConnection();
    bool GrowPool();

private:
    PoolConnections();
    ~PoolConnections();

    std::vector<Connection*> pool_;
    config_t config_params_;

    static PoolConnections* instance_;
    static std::mutex my_mutex_;

};


#endif //FULL_HOUSE_POOLCONNECTIONS_H
