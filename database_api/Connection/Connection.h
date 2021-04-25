#pragma once
#ifndef UNTITLED_CONNECTION_H
#define UNTITLED_CONNECTION_H


#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


#define OK 1
#define DATABASE_NOT_CONNECTED 11


class Connection {
public:
    Connection();
    ~Connection();
    Connection(const Connection&)=default;
    Connection& operator=(const Connection&)=default;

    bool Connect(const std::string &url, const std::string &password,
                 const std::string &user, const std::string &dbase_name);

    bool IsOpen();
    void Close();

    void InitSchema();
    sql::Statement* SetQuery(const std::string& query);
    sql::PreparedStatement* PrepareQuery(const std::string& query);

private:
    sql::Connection* conn_;

};


#endif //UNTITLED_CONNECTION_H
