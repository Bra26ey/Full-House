//
// Created by aleksandr on 15.04.2021.
//

#include <iostream>
#include "TableBoard.h"
#include "Player.h"
#include "Logger.h"

namespace logic {

enum class Combination {
    HIGHEST_CARD = 1,
    ONE_PAIR = 10,
    TWO_PAIRS = 100,
    SET = 10000,
    STRAIGHT = 100000,
    FLUSH = 1000000,
    FULL_HOUSE = 10000000,
    QUADS = 100000000,
    STRAIGHT_FLUSH = 1000000000,
    ROYAL_FLUSH = 1000000000,
};


Player::Player(): in_pot(false), current_stage_money_in_pot(0), position(0) {}



void Player::Fold() {
    in_pot = false;
}

int Player::Call(int to_call) {
    if (to_call > money) {
        int tmp = current_stage_money_in_pot;
        current_stage_money_in_pot += money;
        money = 0;
        return tmp;
    }
    money -= to_call;
    int tmp = current_stage_money_in_pot;
    current_stage_money_in_pot += to_call;
    return tmp;
}

void Player::Check() {
    return;
}

int Player::Raise(int raised_money, int reraise) {
    if (reraise > money) {
        current_stage_money_in_pot += money;
        money = 0;
    } else {
        money -= reraise - current_stage_money_in_pot;
        current_stage_money_in_pot = reraise;
    }
    return current_stage_money_in_pot;
}


int Player::HandValue(std::vector<Card>& board_cards) {
    std::vector<Card> combination_detect;
    for (auto it = board_cards.cbegin(); it != board_cards.cend(); ++it) {
        combination_detect.push_back(*it);
    }
    for (auto it = cards.cbegin(); it != cards.cend(); ++it) {
        combination_detect.push_back(*it);
    }
    std::sort(combination_detect.begin(), combination_detect.end(), [](const Card& l, const Card& r) {
        return l.value == r.value ? l.suit > r.suit : l.value > r.value;
    });

    int value = 0;

    value = RoyalFlush(combination_detect);
    if (value != 0) {
        return value;
    }

    value = StraightFlush(combination_detect);
    if (value != 0) {
        return value;
    }

    value = Quads(combination_detect);
    if (value != 0) {
        return value;
    }

    value = FullHouse(board_cards);
    if (value != 0) {
        return value;
    }

    value = Flush(combination_detect);
    if (value != 0) {
        return value;
    }

    value = Straight(combination_detect);
    if (value != 0) {
        return value;
    }

    value = Set(combination_detect);
    if (value != 0) {
        return value;
    }

    value = TwoPairs(combination_detect);
    if (value != 0) {
        return value;
    }

    value = OnePair(combination_detect);
    if (value != 0) {
        return value;
    }

    value = HighCard(cards);
    if (value != 0) {
        return value;
    }

    return 0;
}

int Player::HighCard(std::vector<Card>& player_cards) {
    return static_cast<int>(player_cards[0].value > player_cards[1].value ? player_cards[0].value : player_cards[1].value) * static_cast<int>(Combination::HIGHEST_CARD);
}

int Player::CountComb(std::vector<Card>& combination_detect, int how_many_comb, int ammount_of_cards) {
    std::vector<Card> buf;
    for (auto it = combination_detect.cbegin(); it != combination_detect.cend(); ++it) {
        buf.push_back(*it);
    }
    int max_value = 0;
    int counter_of_combs = 0;
    int counter_of_current_card = 0;
    while (!buf.empty()) {
        Card current_card = *buf.cbegin();
        buf.erase(buf.begin());
        ++counter_of_current_card;
        while (std::find(buf.cbegin(), buf.cend(), current_card) != buf.cend()) {
            auto it = std::find(buf.cbegin(), buf.cend(), current_card);
            buf.erase(it);
            ++counter_of_current_card;
            if (counter_of_current_card == ammount_of_cards) {
                ++counter_of_combs;
                if (how_many_comb == 2 && ammount_of_cards == 2) {
                    if (max_value == 0) {
                        max_value += static_cast<int>(current_card.value) * 15;
                    } else {
                        max_value += static_cast<int>(current_card.value);
                    }
                } else if (static_cast<int>(current_card.value) > max_value) {
                    max_value = static_cast<int>(current_card.value);
                }

                if (counter_of_combs == how_many_comb) {
                    return max_value;
                }

            }
        }
        counter_of_current_card = 0;
    }

    return 0;
}

int Player::OnePair(std::vector<Card>& combination_detect) {
    return CountComb(combination_detect, 1, 2) * static_cast<int>(Combination::ONE_PAIR);
}

int Player::TwoPairs(std::vector<Card>& combination_detect) {
    return CountComb(combination_detect, 2, 2) * static_cast<int>(Combination::TWO_PAIRS);
}

int Player::Set(std::vector<Card>& combination_detect) {
    return CountComb(combination_detect, 1, 3) * static_cast<int>(Combination::SET);
}

int Player::Straight(std::vector<Card>& combination_detect) {
    int counter = 0;
    for (auto it = combination_detect.begin(); it + 4 != combination_detect.end(); ++it) {
        auto current_straight = it;
        while (static_cast<int>(current_straight->value) == (static_cast<int>((current_straight + 1)->value) + 1)) {
            ++counter;
            ++current_straight;
        }
        if (counter == 4) {
            return static_cast<int>((current_straight - 4)->value) * static_cast<int>(Combination::STRAIGHT);
        }
        counter = 0;
    }
    return 0;
}

int Player::Flush(std::vector<Card>& combination_detect) {
    std::vector<std::pair<int, int> > suits_counter;  // Suit + Max card of Suit
    for (int i = 0; i < 4; ++i) {
        suits_counter.push_back({0, 0});
    }
    for (auto it = combination_detect.cbegin(); it != combination_detect.cend(); ++it) {
        ++suits_counter[static_cast<int>(it->suit)].first;
        if (suits_counter[static_cast<int>(it->suit)].second < static_cast<int>(it->value)) {
            suits_counter[static_cast<int>(it->suit)].second = static_cast<int>(it->value);
        }
    }
    for (auto it = suits_counter.cbegin(); it != suits_counter.cend(); ++it) {
        if (it->first >= 5) {
            return it->second * static_cast<int>(Combination::FLUSH);
        }
    }
    return 0;
}

int Player::FullHouse(std::vector<Card>& combination_detect) {
    int set_value = CountComb(combination_detect, 1, 3);
    if (set_value == 0) {
        return 0;
    }
    std::vector<Card> buf;
    for (auto it = combination_detect.cbegin(); it != combination_detect.cend(); ++it) {
        buf.push_back(*it);
    }
    for (auto it = buf.cbegin(); it != buf.cend(); ++it) {
        if (static_cast<int>(it->value) == set_value) {
            buf.erase(it);
        }
    }
    int pair_value = CountComb(buf, 1, 2);
    if (pair_value == 0) {
        return 0;
    }
    return set_value * static_cast<int>(Combination::FULL_HOUSE);
}

int Player::Quads(std::vector<Card>& combination_detect) {
    return CountComb(combination_detect, 1, 4) * static_cast<int>(Combination::QUADS);
}

int Player::StraightFlush(std::vector<Card>& combination_detect) {
    int straight_value = Straight(combination_detect);
    int flush_value = Flush(combination_detect);
    if ((straight_value & flush_value) == 0) {
        return 0;
    }
    return static_cast<int>(Combination::STRAIGHT_FLUSH) * 2;
}

int Player::RoyalFlush(std::vector<Card>& combination_detect) {
    int straight_flush_value = StraightFlush(combination_detect);
    if (straight_flush_value == 0) {
        return 0;
    }
    for (auto it = combination_detect.cbegin(); it != combination_detect.cend(); ++it) {
        if (it->value == Value::Value_ACE) {
            return static_cast<int>(Combination::ROYAL_FLUSH) * 2;
        }
    }
    return 0;
}

}  // namespace logic
