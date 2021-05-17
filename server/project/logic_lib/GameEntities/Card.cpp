//
// Created by aleksandr on 15.04.2021.
//

#include <string>
#include "Card.h"

namespace logic {

Card::Card(Suit suit, Value value): suit(suit), value(value), show_everyone(false) {}

void Card::Show() {
    show_everyone = true;
}

std::string Card::ToString(const Suit& s, const Value& v) {
    std::string value;
    switch (v) {
        case Value::Value_TWO:   value = "TWO"; break;
        case Value::Value_THREE:   value = "THREE"; break;
        case Value::Value_FOUR: value = "FOUR"; break;
        case Value::Value_FIVE:   value = "FIVE"; break;
        case Value::Value_SIX:   value = "SIX"; break;
        case Value::Value_SEVEN: value = "SEVEN"; break;
        case Value::Value_EIGHT:   value = "EIGHT"; break;
        case Value::Value_NINE:   value = "NINE"; break;
        case Value::Value_TEN: value = "TEN"; break;
        case Value::Value_JACK:   value = "JACK"; break;
        case Value::Value_QUEEN:   value = "QUEEN"; break;
        case Value::Value_KING: value = "KING"; break;
        case Value::Value_ACE: value = "ACE"; break;
    }
    std::string suit;
    switch (s) {
        case Suit::DIAMONDS: suit = "DIAMONDS"; break;
        case Suit::HEARTS: suit = "HEARTS"; break;
        case Suit::SPADES: suit = "SPADES"; break;
        case Suit::CLUBS: suit = "CLUBS"; break;
    }
    return value + " OF " + suit;
}

bool Card::operator==(const Card& card) const {
    return static_cast<int>(value) == static_cast<int>(card.value);
}

}  // namespace logic
