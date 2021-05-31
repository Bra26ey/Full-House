#pragma once

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <mutex>

#include "FileHandler.h"

namespace database {

class Connection {
public:
    Connection();
    ~Connection();
    Connection(const Connection&)=default;
    Connection& operator=(const Connection&)=default;
    Connection& operator=(Connection&& c) noexcept;

    bool Connect(const std::string &url, const std::string &password,
                 const std::string &user, const std::string &dbase_name);

    bool IsOpen();
    void Close();

    int InitSchema();
    sql::Statement* SetQuery(const std::string& query);
    sql::PreparedStatement* PrepareQuery(const std::string& query);

private:
    std::shared_ptr<sql::Connection> conn_;
    db_schemas_t schemas_;

};

}  // namespace database
