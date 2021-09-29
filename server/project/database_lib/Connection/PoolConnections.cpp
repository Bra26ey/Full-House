#include "PoolConnections.h"

namespace database {

std::unique_ptr<PoolConnections> PoolConnections::instance_(nullptr);
std::mutex PoolConnections::my_mutex_;

PoolConnections::PoolConnections() {
    for (std::size_t i = 0; i < POOL_SIZE; ++i) {
        Connection conn;
        pool_.emplace_back(std::pair<Connection, int> (conn, FREE_CONN));
    }
}


PoolConnections* PoolConnections::GetInstance() {
    std::lock_guard<std::mutex> lock(my_mutex_);
    if (instance_ == nullptr) {
        instance_ = std::unique_ptr<PoolConnections>(new PoolConnections());
    }
    return instance_.get();
}


int PoolConnections::SetParams(const std::string& filename = "config.txt") {
    std::lock_guard<std::mutex> lock(my_mutex_);
    FileHandler fh;
    config_params_ = fh.ParseConfig(filename);
    return static_cast<int>(config_params_.status_code);
}


int PoolConnections::GetConnection(Connection& conn) {
    std::lock_guard<std::mutex> lock(my_mutex_);
    if (pool_.empty()) {
        return false;
    }

    std::size_t old_size = pool_.size();
    int ind = 0;
    for (auto & elem : pool_) {
        if (!elem.first.IsOpen() && elem.second == FREE_CONN) {
            bool out = elem.first.Connect(config_params_.url, config_params_.password,
                                          config_params_.user, config_params_.database);
            if (out) {
                conn = std::move(elem.first);
                elem.second = BUSY_CONN;
                return ind;
            }
            else {
                return -1;
            }
        }
        ++ind;
    }

    if (!GrowPool()) {
        std::cout << "You've reached max pool size!" << std::endl;
        return false;
    }

    bool out = pool_[old_size].first.Connect(config_params_.url, config_params_.password,
                                             config_params_.user, config_params_.database);
    if (out) {
        conn = std::move(pool_[old_size].first);
        pool_[old_size].second = BUSY_CONN;
        return static_cast<int>(old_size);
    }
    else {
        return -1;
    }
}


void PoolConnections::ReturnConnection(Connection& conn, int& ind) {
    pool_[ind].first = std::move(conn);
    pool_[ind].first.Close();
    pool_[ind].second = FREE_CONN;
    ind = -1;
}


bool PoolConnections::GrowPool() {
    std::size_t pool_size = pool_.size();
    if (pool_size >= MAX_POOL_SIZE) {
        return false;
    }
    pool_.resize(2 * pool_size);
    std::generate(pool_.begin(), pool_.end(), [](){
        Connection conn;
        return std::make_pair(conn, FREE_CONN);
    });
    return true;
}

}  // namespace database
