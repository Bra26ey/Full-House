#pragma once
#ifndef FULL_HOUSE_USER_H
#define FULL_HOUSE_USER_H


#include "../Connection/PoolConnections.h"


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
    explicit User(Connection*& conn) : conn_(conn) {}
    ~User() { conn_ = nullptr; }

    User(const User&)=delete;
    User& operator=(const User&)=delete;

    user_t GetUser(const std::size_t&, bool with_password);
    user_t GetUser(const std::string&, bool with_password);

    std::pair<std::size_t, int> InsertUser(const std::string&, const std::string&);
    int UpdateLogin(const std::size_t&, const std::string&);
    int UpdatePassword(const std::size_t&, const std::string&);
    int UpdateUsername(const std::size_t&, const std::string&);
    int UpdateAvatar(const std::size_t&, const std::string&);
    int UpdateMoney(const std::size_t&, double);
    int DeleteUser(const std::size_t&);
    bool IsExist(const std::size_t&);
    bool IsExist(const std::string &);

private:
    int UpdateStringField(const std::string&, const std::size_t &, const std::string&);

    Connection* conn_;

};


#endif //FULL_HOUSE_USER_H
