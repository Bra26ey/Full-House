#include "PoolConnections.h"


PoolConnections::PoolConnections() {
    //driver_ = get_driver_instance();
    for (std::size_t i = 0; i < POOL_SIZE; ++i) {
        pool_.push_back(new Connection());
    }
}


PoolConnections::~PoolConnections() {
    for (std::size_t i = 0; i < pool_.size(); ++i) {
        if (pool_[i] != nullptr) {
            pool_[i]->Close();
            delete pool_[i];
        }
    }
}


PoolConnections* PoolConnections::GetInstance() {
    std::lock_guard<std::mutex> lock(my_mutex_);
    if (instance_ == nullptr) {
        instance_ = new PoolConnections();
    }
    return instance_;
}


void PoolConnections::SetParams(const std::string &url, const std::string &password,
                                const std::string &user, const std::string &dbase_name) {
    url_ = url;
    user_ = user;
    password_ = password;
    dbase_name_ = dbase_name;
}


Connection* PoolConnections::GetConnection() {
    std::lock_guard<std::mutex> lock(my_mutex_);
    if (pool_.empty()) {
        return nullptr;
    }

    std::size_t old_size = pool_.size();
    for (auto & elem : pool_) {
        if (!elem->IsOpen()) {
            bool out = elem->Connect(url_, password_, user_, dbase_name_);
            return out ? elem : nullptr;
        }
    }

    if (!GrowPool()) {
        std::cout << "You've reached max pool size!" << std::endl;
        return nullptr;
    }

    bool out = pool_[old_size]->Connect(url_, password_, user_, dbase_name_);
    return out ? pool_[old_size] : nullptr;
}


bool PoolConnections::GrowPool() {
    std::size_t pool_size = pool_.size();
    if (pool_size >= MAX_POOL_SIZE) {
        return false;
    }
    pool_.resize(2 * pool_size);
    for (std::size_t i = pool_size; i < pool_.size(); ++i) {
        pool_[i] = new Connection();
    }
    return true;
}
