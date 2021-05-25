#pragma once

#include <array>
#include <mutex>
#include <string>

constexpr uint8_t TABLE_SIZE = 6;

constexpr uint8_t TPOS_ERROR = __UINT8_MAX__;

struct Position {
    bool is_empty = true;
    uint64_t id;
};


class TablePositions {
 public:
    TablePositions() = default;
    ~TablePositions() = default;

    uint8_t Insert(uint64_t const &id);
    void Delete(uint64_t const &id);
    uint8_t GetPosition(uint64_t const &id);
    uint64_t GetId(uint8_t const &position);

 private:
    std::array<Position, TABLE_SIZE> positions;
    std::mutex mutex;
};
