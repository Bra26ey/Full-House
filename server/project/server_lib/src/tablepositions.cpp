#include "tablepositions.h"

#include <algorithm>

uint8_t TablePositions::Insert(uint64_t const &id) {
    const std::lock_guard<std::mutex> lock(mutex);

    auto it = std::find_if(positions.begin(), positions.end(),
                           [](const Position &current) { return current.is_empty; });

    if (it == positions.end()) {
        return TPOS_ERROR;
    }
    
    it->id = id;
    it->is_empty = false;

    return std::distance(positions.begin(), it);
}

void TablePositions::Delete(uint64_t const &id) {
    mutex.lock();
    auto it = std::find_if(positions.begin(), positions.end(),
                           [id](const Position &current) { return current.id == id; });
    if (it == positions.end()) {
        return;
    }

    it->is_empty = true;
    mutex.unlock();
}

uint8_t TablePositions::GetPosition(uint64_t const &id) {
    const std::lock_guard<std::mutex> lock(mutex);

    auto it = std::find_if(positions.begin(), positions.end(),
                           [id](const Position &current) { return current.id == id; });

    if (it == positions.end()) {
        return TPOS_ERROR;
    }

    return std::distance(positions.begin(), it);
}

uint64_t TablePositions::GetId(uint8_t const &position) {
    const std::lock_guard<std::mutex> lock(mutex);
    return positions[position].id;
}



