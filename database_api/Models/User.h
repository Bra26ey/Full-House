#pragma once
#ifndef UNTITLED_USER_H
#define UNTITLED_USER_H


#include "../Connection/PoolConnections.h"

// set constants
#define OBJECT_ALREADY_EXIST 21
#define OBJECT_NOT_EXIST 22
#define OBJECT_NOT_UPDATED 23
#define EMPTY_DATA 24

#define DEFAULT_PATH_TO_IMAGE "default_path_to_image"


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

    user_t GetUser(const std::string& login, bool with_password);
    int InsertUser(const std::string&, const std::string&);
    int UpdateLogin(const std::string&, const std::string&);
    int UpdatePassword(const std::string&, const std::string&);
    int UpdateUsername(const std::string&, const std::string&);
    int UpdateAvatar(const std::string&, const std::string&);
    int UpdateMoney(const std::string&, double);
    int DeleteUser(const std::string&);
    bool IsExist(const std::string&);

private:
    int UpdateStringField(const std::string&, const std::string&, const std::string&);

    Connection* conn_;

};


#endif //UNTITLED_USER_H
