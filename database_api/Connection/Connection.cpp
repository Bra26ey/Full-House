#include "Connection.h"


Connection::Connection() : conn_(nullptr) {}


Connection::~Connection() { Close(); }


bool Connection::Connect(const std::string &url, const std::string &password,
             const std::string &user, const std::string &dbase_name) {
    if (conn_ == nullptr || conn_->isClosed()) {
        try {
            sql::Driver *driver = get_driver_instance();
            conn_ = driver->connect(url, user, password);
            conn_->setSchema(dbase_name);

            InitSchema();
            return true;
        } catch (sql::SQLException & e) {
            std::cout << "Could not to connect to database '"<< dbase_name << "'" << std::endl;
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
    delete conn_;
    conn_ = nullptr;
}


void Connection::InitSchema() {
    sql::Statement *stmt;
    stmt = conn_->createStatement();

    // create base table
    stmt->execute("CREATE TABLE IF NOT EXISTS user(id INT NOT NULL AUTO_INCREMENT, login VARCHAR(20) NOT NULL, password VARCHAR(20) NOT NULL, username VARCHAR(20) NOT NULL, avatar VARCHAR(255), money DOUBLE NOT NULL, PRIMARY KEY (id), UNIQUE (login));");
    stmt->execute("CREATE TABLE IF NOT EXISTS board(board_id INT NOT NULL AUTO_INCREMENT, player_id INT NOT NULL, reserved_money DOUBLE, PRIMARY KEY (board_id, player_id));");

    delete stmt;
}


sql::Statement* Connection::SetQuery(const std::string& query) {
    return conn_->createStatement();
}


sql::PreparedStatement* Connection::PrepareQuery(const std::string& query) {
    return conn_->prepareStatement(query);
}
