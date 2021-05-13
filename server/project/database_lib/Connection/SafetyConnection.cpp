#include "SafetyConnection.h"


namespace database {

SafetyConnection::SafetyConnection() {
    source_ind_ = -1;
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("config.txt");
    source_ind_ = instance->GetConnection(conn_);
}


SafetyConnection::~SafetyConnection() {
    ReturnConnection();
}


void SafetyConnection::Lock() {
    conn_mutex_.lock();
}


void SafetyConnection::Unlock() {
    conn_mutex_.unlock();
}


Connection& SafetyConnection::GetConnection() {
    return conn_;
}


bool SafetyConnection::IsConnected() {
    return source_ind_ != -1 && conn_.IsOpen();
}

void SafetyConnection::ReturnConnection() {
    if (source_ind_ == -1) {
        return;
    }
    PoolConnections *instance = PoolConnections::GetInstance();
    instance->ReturnConnection(conn_, source_ind_);
}

}  // namespace database
