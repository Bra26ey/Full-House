#ifndef FULL_HOUSE_SAFETYCONNECTION_H
#define FULL_HOUSE_SAFETYCONNECTION_H


#include "PoolConnections.h"


class SafetyConnection {
public:
    SafetyConnection();
    ~SafetyConnection();

    void Lock();
    void Unlock();

    Connection& GetConnection();
    bool IsConnected();
    void ReturnConnection();

private:
    Connection conn_;
    int source_ind_;
    std::mutex conn_mutex_;

};


#endif //FULL_HOUSE_SAFETYCONNECTION_H
