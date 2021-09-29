#pragma once
#ifndef FULL_HOUSE_USER_H
#define FULL_HOUSE_USER_H


#include "PoolConnections.h"
#include "SafetyConnection.h"

#include "Constants.h"

namespace database {

typedef struct user user_t;
struct user {
    size_t id;
    std::string login;
    std::string password;
    std::string username;
    std::string avatar;
    double money;
    size_t status_code;

    user() : id(0), money(0.0), status_code(EMPTY_DATA) {}
    ~user()=default;

    user(const user_t& u)=default;
    user& operator=(const user_t& u)=default;
};


class User {
public:
    User()=default;
    ~User()=default;

    User(const User&)=delete;
    User& operator=(const User&)=delete;

    user_t GetUser(const std::size_t&, bool with_password = false);
    user_t GetUser(const std::string&, bool with_password = false);

    // registration
    std::pair<std::size_t, int> RegUser(const std::string&, const std::string&);
    // authorization
    std::pair<user_t, int> AuthUser(const std::string& login, const std::string& password);
    int UpdateLogin(const std::size_t&, const std::string&);
    int UpdatePassword(const std::size_t&, const std::string&);
    int UpdateUsername(const std::size_t&, const std::string&);
    int UpdateAvatar(const std::size_t&, const std::string&);
    int UpdateMoney(const std::size_t&, const double& new_money);
    int UpdateMoneyByDelta(const std::size_t&, const double& delta_money);

    int DeleteUser(const std::size_t&);
    bool IsExist(const std::size_t&);
    bool IsExist(const std::string &);

private:
    std::pair<std::size_t, int> InsertUser(const std::string&, const std::string&);
    int UpdateStringField(const std::string&, const std::size_t &, const std::string&);
    int UpdateMoney(const size_t &id, const double& money, const std::string& query_string);

    SafetyConnection conn_;

};

}  // namespace database

#endif //FULL_HOUSE_USER_H
