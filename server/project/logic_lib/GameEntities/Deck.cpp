//
// Created by aleksandr on 17.04.2021.
//

#include "Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck(int size): size(size) {
    for (int i = 0; i < size; ++i) {
        deck.push_back({static_cast<Suit>(i / 13), static_cast<Value>(i % 13 + 2)});  // suit and value
    }
    deck.shrink_to_fit();
}

void Deck::Shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(std::begin(deck), std::end(deck), rng);
}

Card& Deck::Peak() {
    return deck[0];
}

void Deck::Erase() {
    deck.erase(deck.begin());
}