#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <filesystem>

#include "Constants.h"

namespace database {

typedef struct config config_t;
struct config {
    std::string url;
    std::string user;
    std::string password;
    std::string database;
    std::size_t status_code;

    config() : status_code(EMPTY_DATA) {}
    ~config()=default;
    config(const config&)=default;
    config& operator=(const config&)=default;
};


typedef struct db_schemas db_schemas_t;
struct db_schemas {
    std::string user;
    std::string board;
    std::string active_board;
    std::size_t status_code;

    db_schemas() : status_code(EMPTY_DATA) {}
    ~db_schemas()=default;
    db_schemas(const db_schemas&)=default;
    db_schemas& operator=(const db_schemas&)=default;
};


class FileHandler {
public:
    FileHandler();
    ~FileHandler()=default;

    std::map<std::string, std::string> Parse(const std::string& filename);
    config_t ParseConfig(const std::string& filename = "config.txt");
    db_schemas_t ParseDbSchema(const std::string& filename = "db_schema.txt");

private:
    void Fill(const std::string &name, const std::string &source, config_t& cnfg);
    void Fill(const std::string &name, const std::string &source, db_schemas_t &dbscm);
    bool EmptyField(const config_t &cnfg);
    bool EmptyField(const db_schemas_t &dbscm);

    std::string absolute_path_;

};

}  // namespace database
