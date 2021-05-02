#include "PoolConnections.h"


PoolConnections::PoolConnections() {
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


int PoolConnections::SetParams(const std::string& filename = "config.txt") {
    FileHandler fh;
    config_params_ = fh.ParseConfig(filename);
    return (int) config_params_.status_code;
}


Connection* PoolConnections::GetConnection() {
    std::lock_guard<std::mutex> lock(my_mutex_);
    if (pool_.empty()) {
        return nullptr;
    }

    std::size_t old_size = pool_.size();
    for (auto & elem : pool_) {
        if (!elem->IsOpen()) {
            bool out = elem->Connect(config_params_.url, config_params_.password,
                                     config_params_.user, config_params_.database);
            return out ? elem : nullptr;
        }
    }

    if (!GrowPool()) {
        std::cout << "You've reached max pool size!" << std::endl;
        return nullptr;
    }

    bool out = pool_[old_size]->Connect(config_params_.url, config_params_.password,
                                        config_params_.user, config_params_.database);
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
