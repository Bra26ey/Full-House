//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <string>

namespace logic {

enum class Suit {
    DIAMONDS,
    HEARTS,
    CLUBS,
    SPADES,
};

enum class Value {
    Value_TWO = 2,
    Value_THREE = 3,
    Value_FOUR = 4,
    Value_FIVE = 5,
    Value_SIX = 6,
    Value_SEVEN = 7,
    Value_EIGHT = 8,
    Value_NINE = 9,
    Value_TEN = 10,
    Value_JACK = 11,
    Value_QUEEN = 12,
    Value_KING = 13,
    Value_ACE = 14,
};




struct Card {
    Suit suit;
    Value value;
    bool show_everyone = false;

    Card(Suit suit, Value value);
    bool operator==(const Card& card) const;

    void Show();
    static std::string ToString(const Suit& s, const Value& v);
};

}  // namespace logic
