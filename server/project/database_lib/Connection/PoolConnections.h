#pragma once

#include <iostream>
#include <string>
#include <future>
#include <mutex>
#include <vector>
#include <utility>

#include "Connection.h"

namespace database {

class PoolConnections {
public:
    // prohibit copying the object
    PoolConnections(const PoolConnections&) = delete;
    Connection operator=(const PoolConnections&) = delete;

    static PoolConnections* GetInstance();

    int SetParams(const std::string& filename);
    int GetConnection(Connection& conn);
    void ReturnConnection(Connection& conn, int& ind);

private:
    PoolConnections();
    ~PoolConnections()=default;

    bool GrowPool();

    std::vector<std::pair<Connection, bool>> pool_;
    config_t config_params_;

    static PoolConnections* instance_;
    static std::mutex my_mutex_;

};

}  // namespace database
