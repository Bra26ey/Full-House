#include "Connection.h"

namespace database {

Connection::Connection() : conn_(nullptr) {}


Connection::~Connection() {
    Close();
}


Connection& Connection::operator=(Connection&& c) noexcept {
    conn_ = c.conn_;
    c.conn_ = nullptr;
    return *this;
}


bool Connection::Connect(const std::string &url, const std::string &password,
                         const std::string &user, const std::string &dbase_name) {
    if (conn_ == nullptr || conn_->isClosed()) {
        try {
            sql::Driver *driver = get_driver_instance();
            conn_.reset(driver->connect(url, user, password));
            conn_->setSchema(dbase_name);

            return InitSchema() == OK;
        } catch (sql::SQLException &e) {
            std::cout << "Could not to connect to database '" << dbase_name << "'" << std::endl;
            return false;
        }
    }
    return false;
}


bool Connection::IsOpen() {
    return conn_ != nullptr && !conn_->isClosed();
}


void Connection::Close() {
    if (conn_ == nullptr) {
        return;
    }

    if (!conn_->isClosed()) {
        conn_->close();
    }
}


int Connection::InitSchema() {
    if (schemas_.status_code == EMPTY_DATA) {
        FileHandler fh;
        schemas_ = fh.ParseDbSchema();
        if (schemas_.status_code != OK) {
            return static_cast<int>(schemas_.status_code);
        }
    }

    sql::Statement *stmt;
    stmt = conn_->createStatement();

    // create base table
    stmt->execute(schemas_.user);
    stmt->execute(schemas_.board);
    stmt->execute(schemas_.active_board);

    delete stmt;
    return static_cast<int>(schemas_.status_code);
}


sql::Statement* Connection::SetQuery(const std::string& query) {
    return conn_->createStatement();
}


sql::PreparedStatement* Connection::PrepareQuery(const std::string& query) {
    return conn_->prepareStatement(query);
}

}  // namespace database
