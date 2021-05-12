#include "rulesfragment.h"

#include <QScrollArea>

RulesFragment::RulesFragment() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    QFrame *centerConainer = new QFrame;

    QVBoxLayout *startMainLayout = new QVBoxLayout;
    QHBoxLayout *startContent = new QHBoxLayout;

    QVBoxLayout *backContainer = new QVBoxLayout;

    QVBoxLayout *buttonContainer = new QVBoxLayout;
    QVBoxLayout *controlButtons = new QVBoxLayout;


    ControlButton = new QPushButton("Next Page");
    ControlButton->setStyleSheet("color:#242424;font-size:24px");
    connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);


    ExitButton = new QPushButton("Back");
    ExitButton->setStyleSheet("color:#242424;font-size:24px");
    connect(ExitButton, &QPushButton::clicked, this, &RulesFragment::onExitPressed);

    Rules1 = new QLabel;
    Rules1->setText(" 1\n"
"Learn the 10 basic 5-card hands and their ranking. No matter what type of poker you play, the hands will always be the same. \n"
"Here are the winning poker hands, from highest to lowest:\n"
"   A) The highest-ranking hand is a royal flush (the royal straight flush).\n"
"This hand includes a 10, Jack, Queen, King, and Ace of the same suit, one kind (all clubs, diamonds, hearts or spades).\n"
"It can only be tied but not beaten by the royal flush of another suit.\n"
"   B) A straight flush is made up of 5 consecutive cards of the same suit.\n"
"   C) 4 of a kind means you have 4 cards of the same rank (but different suits, of course) and a fifth card of any rank (such as 4 aces and a 9). If you\n"
"have 4 aces, then no one can have any hand with an ace, so that no royal flush is available.\n"
"   D) A full house contains 3 matching cards of 1 rank and 2 matching cards of another rank.\n"
"   E) A flush contains any 5 cards of the same suit. These skip around in rank or sequence, but are from the same suit.\n"
"   F) A straight contains 5 cards of consecutive rank but from more than one suit.\n"
"   G) 3 of a kind means you have 3 cards of the same rank, plus two unmatched cards.\n"
"   H) 2 pair is made up of two cards of one rank, plus two cards of another rank (different from the first pair), plus one unmatched card.\n"
"Pair means you have 2 cards of the same rank, plus 3 other unmatched cards.\n"
"   J) High card is the lowest-ranking hand, when no two cards have the same rank,\n"
"the five cards are not consecutive, and they are not all from the same suit.\n\n"


"2\n"
"Place the blinds (starting bets) or ante up. In Texas Hold’em, the player next to the dealer typically places a small blind bet that’s half of the\n"
" usual minimum bet, while the player to that person’s left places a big blind that’s at least the minimum bet. \n\n"

"3\n"
"Look at the 2 cards the dealer gives you, which is your hand. Then, they'll pass out 2 cards to each player. Check your cards to see what you're holding.\n"
"The dealer will always pass out the cards in a clockwise direction, starting on the left.\n\n"


"4\n"
"Bet, call, or raise after each round is dealt if you want to. Every time the dealer puts out new cards, you'll make a bet, with the first bet being made\n"
"solely based on the two cards the players have in their hands. Betting happens in a circle – when it's your turn to bet, you have a few options:\n"
"   Place an initial bet if no one else has yet.\n"
"   Say check to avoid betting.\n"
"   Say call to match the bet someone else has made.\n"
"   Say raise to add more money to the betting pool. If you raise, the other players will go around in a circle and choose to either call your new bet or fold.\n"
"   Say fold if someone else has bet and you don't want to match their bet. If you fold, turn your cards into the dealer face-down.\n");

Rules2 = new QLabel;
Rules2->setText("5\n"
"Look at the flop to see if you have a good hand. Then, they'll put 3 cards face-up on the table, which is called the flop. These are the community cards that\n"
"every player can use to build their hand. Compare these cards and the cards in your hand, then place a bet, call a bet, or fold. In total, the dealer will\n"
"reveal 5 cards. You will have 7 cards total to use to create your best hand of 5: your two personal cards in your hands, and the five community cards on the\n"
"table. Take some time to analyze the table after the flop – are you well-positioned to end the game with a good hand? Depending on the rules where you’re\n"
"playing, you can also draw replacement cards for the cards in your hand. This is usually done during or just after the betting round.\n\n"


"6\n"
"Check the “turn” card after the second round of betting. The dealer will burn the top card, then they'll place 1 card face up next to the flop. \n"
"Check all of the cards on the table and the cards in your hand to see if you want to bet, call, or raise.\n"
"Your game may also allow a card exchange at this point, but this isn’t typical in professional games.\n"
"As you look at the cards, think about the possible hands other players might have. For instance, if all 4 cards on the table are spades,\n"
"then any player who has a spade in their hand will have a flush, which means they have 5 cards from the same house.\n"
"Similarly, if the cards on the table are 5,6,7, and 8, then anyone with a 4 or 9 will have a straight.\n"
"If you have nothing good in your hand but the cards on the table make for an easy winning hand, then you may want to fold.\n\n"


"7\n"
"Examine the 'river' card and decide on the hand you'll play. After the dealer 'burns' the top card on the deck, they'll put 1 last card\n"
"face up next to the 'turn' card. This final card is called the “river.” Check your hand and the community cards to decide on your best 5-card hand.\n"
"Then, bet, call, or fold.\n\n"


"8\n"
"Reveal each player’s hand clockwise in the final “showdown.” After each player calls, folds, or bets in the last round, each remaining player will participate\n"
"in the 'showdown. Starting to the left of the dealer, all players involved will reveal their cards face up. Then, everyone looks at the turned over hands to\n"
"see who has the highest value hand to win the entire pot. If there is a tie, the tied players split the pot. If you fold your hand, then you don’t have to show\n"
"your cards. In Texas Hold’em, there are 5 cards on the table and 2 cards in your hand. You can create any 5-card combination using these 7 cards.\n"
"The remaining cards aren’t counted.");

    Rules1->setStyleSheet("font-family: sans-serif;font-size:20px;background-color:rgba(164, 191, 67, 0.6); border-radius:20px;margin:5px");
    Rules2->setStyleSheet("font-family: sans-serif;font-size:20px;background-color:rgba(164, 191, 67, 0.6); border-radius:20px;margin:5px");
    Rules2->hide();
    controlButtons->addWidget(ControlButton);
    buttonContainer->addWidget(ExitButton);

    startContent->setContentsMargins(46,46,46,46);
    startContent->addWidget(Rules1);
    startContent->addWidget(Rules2);

    centerConainer->setStyleSheet("color:#242424;font-size:24px");
    centerConainer->setLayout(startContent);

    backContainer->setAlignment(Qt::AlignTop);
    startContent->addLayout(backContainer);

    startContent->addLayout(startMainLayout);

    mainHLayout->addWidget(centerConainer);
    mainHLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->addLayout(controlButtons);
    mainVLayout->addLayout(buttonContainer);
    mainVLayout->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVLayout);
}

void RulesFragment::onNextPressed() {
    ControlButton->disconnect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);
    ControlButton->connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onPrevPressed);
    ControlButton->setText("Previous Page");
    Rules1->hide();
    Rules2->show();
}

void RulesFragment::onPrevPressed() {
    ControlButton->disconnect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onPrevPressed);
    ControlButton->connect(ControlButton, &QPushButton::clicked, this, &RulesFragment::onNextPressed);
    ControlButton->setText("Next Page");
    Rules2->hide();
    Rules1->show();
}

void RulesFragment::onExitPressed() {
    back();
}

RulesFragment::~RulesFragment() {
    delete Rules1;
    delete Rules2;
    delete ControlButton;
    delete ExitButton;
}
