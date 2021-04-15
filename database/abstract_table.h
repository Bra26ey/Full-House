#ifndef CPP_PROJECT_ABSTRACT_TABLE_H
#define CPP_PROJECT_ABSTRACT_TABLE_H

#include "base_database.h"

#include <string>

typedef struct Info info_t;
struct Info {

};


class AbstractTable {
protected:

    // info_t - должен быть шаблоном, принимающим производные классы
    // тогда, по идее, поведение функций меняться не должно
    int Select(std::string&, info_t&) {

        return 0;
    }

    int Insert(std::string&, info_t&) {
        return 0;
    }

    int Update(std::string&, info_t&) {
        return 0;
    }

    int Delete(std::string&, info_t&) {
        return 0;
    }

private:
    BaseDB db;

};


class User : protected AbstractTable {
public:
    User() : id_(0), login_(""), password_("") {}
    User& operator=(const User& rhs) {
        if (this == &rhs) {
            return *this;
        }

        id_ = rhs.id_;
        login_ = rhs.login_;
        password_ = rhs.password_;
        return *this;
    }

    int UpdateUser() { return 0; }
    int UpdateLogin() { return 0; }
    int UpdatePassword() { return 0; }
    std::string GetLogin() { return login_; }
    std::string GetPassword() { return password_; }

private:
    size_t id_;
    std::string login_;
    std::string password_;

};


class Profile : protected AbstractTable {
public:
    int UpdateProfile() { return 0; }
    int UpdateUsername(const std::string& new_username) { return 0; }
    int UpdateAvatar(const std::string& new_avatar) { return 0; }
    int UpdateMoney(float new_money) { return 0; }

    std::string& GetUsername() { return username_; }
    std::string& GetAvatar() { return avatar_; }
    float GetMoney() { return money_; }

private:
    size_t id_;
    std::string username_;
    std::string avatar_;
    float money_;

};


class Player : protected AbstractTable {
public:
    int SavePlayer() { return 0; }
    int GetCards() { return 0; }
    int UpdateCards() { return 0; };

private:
    int table_id_;
    std::pair<std::string, std::string>* cards_;

};


class Board : protected AbstractTable {
public:
    friend Player;

    int SaveBoard() { return 0; }
private:
    int id_;
    Player* players_;

};





#endif //CPP_PROJECT_ABSTRACT_TABLE_H
