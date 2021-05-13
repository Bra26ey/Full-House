#include "tablepositions.h"

#include <algorithm>

uint8_t TablePositions::Insert(std::string const &name) {
    const std::lock_guard<std::mutex> lock(mutex);

    auto it = std::find_if(positions.begin(), positions.end(),
                           [](const Position &current) { return current.is_empty; });

    if (it == positions.end()) {
        return TPOS_ERROR;
    }
    
    it->name = name;
    it->is_empty = false;

    return std::distance(positions.begin(), it);
}

void TablePositions::Delete(std::string const &name) {
    mutex.lock();
    auto it = std::find_if(positions.begin(), positions.end(),
                           [name](const Position &current) { return current.name == name; });
    if (it == positions.end()) {
        return;
    }

    it->is_empty = true;
    mutex.unlock();
}

uint8_t TablePositions::GetPosition(std::string const &name) {
    const std::lock_guard<std::mutex> lock(mutex);

    auto it = std::find_if(positions.begin(), positions.end(),
                           [name](const Position &current) { return current.name == name; });

    if (it == positions.end()) {
        return TPOS_ERROR;
    }

    return std::distance(positions.begin(), it);
}



