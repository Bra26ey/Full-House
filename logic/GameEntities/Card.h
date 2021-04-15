//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_CARD_H
#define FULL_HOUSE_CARD_H

enum class Suit {
    DIAMONDS,
    HEARTS,
    CLUBS,
    SPADES,
};

enum class Value {
    Value_TWO = 2,
    Value_THREE,
    Value_FOUR,
    Value_FIVE,
    Value_SIX,
    Value_SEVEN,
    Value_EIGHT,
    Value_NINE,
    Value_TEN,
    Value_JACK,
    Value_QUEEN,
    Value_KING,
    Value_ACE,
};


struct Card {
    unsigned suit: 2;
    unsigned value: 4;
    bool show_everyone = false;

    void Show();
};

#endif //FULL_HOUSE_CARD_H
