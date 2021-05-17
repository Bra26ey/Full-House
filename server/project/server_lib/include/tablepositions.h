#pragma once

#include <array>
#include <mutex>
#include <string>

constexpr uint8_t TABLE_SIZE = 6;

constexpr uint8_t TPOS_ERROR = __UINT8_MAX__;

struct Position {
    bool is_empty = true;
    std::string name;
};


class TablePositions {
 public:
    TablePositions() = default;
    ~TablePositions() = default;

    uint8_t Insert(std::string const &name);
    void Delete(std::string const &name);
    uint8_t GetPosition(std::string const &name);

 private:
    std::array<Position, TABLE_SIZE> positions;
    std::mutex mutex;
};