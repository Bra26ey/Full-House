#pragma once

#include "PoolConnections.h"


namespace database {

class SafetyConnection {
public:
    SafetyConnection();
    ~SafetyConnection();

    void Lock();
    void Unlock();
    std::mutex& GetMutex() {
        return conn_mutex_;
    }

    Connection& GetConnection();
    bool IsConnected();
    void ReturnConnection();

private:
    Connection conn_;
    int source_ind_;
    std::mutex conn_mutex_;

};

}  // namespace database
