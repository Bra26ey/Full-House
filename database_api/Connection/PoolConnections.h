#ifndef UNTITLED_DATABASEAPI_H
#define UNTITLED_DATABASEAPI_H


#include <iostream>
#include <string>
#include <future>
#include <mutex>
#include <memory>

#include "Connection.h"


#define POOL_SIZE 10
#define MAX_POOL_SIZE 40


class PoolConnections {
public:
    // prohibit copying the object
    PoolConnections(const PoolConnections&) = delete;
    PoolConnections& operator=(const PoolConnections&) = delete;

    static PoolConnections* GetInstance();
    void SetParams(const std::string &url, const std::string &password,
                   const std::string &user, const std::string &dbase_name);
    Connection* GetConnection();
    bool GrowPool();

protected:
    std::vector<Connection*> pool_;

    std::string url_;
    std::string user_;
    std::string password_;
    std::string dbase_name_;

private:
    PoolConnections();
    ~PoolConnections();

    static PoolConnections* instance_;
    static std::mutex my_mutex_;

};


#endif //UNTITLED_DATABASEAPI_H
