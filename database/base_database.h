#ifndef CPP_PROJECT_BASE_DATABASE_H
#define CPP_PROJECT_BASE_DATABASE_H


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <soci/soci.h>
#include <soci/connection-pool.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <string>


class BaseDB {
public:
    BaseDB() : instance_(nullptr), size_(0) {}
    ~BaseDB() { Close(); }

    soci::connection_pool* GetInstance() { return instance_; }

    bool Connect(const std::string& conn_str, std::size_t n = 5) {
        if (instance_ != nullptr) {
            Close();
        }

        int is_connected = 0;
        if (instance_ != new soci::connection_pool((size_ = n))) {
            return false;
        }

        try {
            soci::indicator ind;
            for (std::size_t i = 0; i < size_; ++i) {
                soci::session& sql = instance_->at(i);
                sql.open(conn_str);
                sql << "SELECT 1;", soci::into(is_connected, ind);
                if (!is_connected) {
                    break;
                } else if (i + 1 < size_) {
                    is_connected = 0;
                }
            }
        } catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }

        if (!is_connected) {
            Close();
        }

        return (instance_ != nullptr);
    }

    void Close() {
        if (instance_ != nullptr) {
            try {
                for (std::size_t i = 0; i < size_; ++i) {
                    soci::session& sql = instance_->at(i);
                    sql.close();
                }
                delete instance_;
                instance_ = nullptr;
            } catch (std::exception const& e) {
                std::cerr << e.what() << std::endl;
            }
            size_ = 0;
        }
    }

private:
    soci::connection_pool* instance_;
    std::size_t size_;

};


#endif //CPP_PROJECT_BASE_DATABASE_H
