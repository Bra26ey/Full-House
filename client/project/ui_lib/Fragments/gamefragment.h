#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"
#include "otherplayer.h"
#include "playtable.h"
#include "card.h"
#include "dealerlogo.h"
#include "player.h"
#include "turnspark.h"
#include "winlabel.h"
#include "client_impl.h"

#include <QVector>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>



class GameFragment: public BaseFragment {
    Q_OBJECT
public:
    GameFragment();
    ~GameFragment();
    void DrawPlayer(size_t player_id, std::string nickname, size_t total_money);
    void DrawMainPlayer();
    void MakeDealer(size_t player_id);
    void FlipAllCards();

    void DisplayWinner(OtherPlayer* winner);
    void CurrentTurn(OtherPlayer* player);
    void DeleteWinnerDisplay();

    void AddCardToTable(size_t value, size_t suit, bool upsided);
    void DeleteAllCardsFromTable();

    void GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2);
    void FlipCards(size_t player_id);
    void SetBet(size_t player_id, size_t bet);
    void SetFold(size_t player_id);
    void SetCall(size_t player_id);
    void SetRaise(size_t player_id);
    void SetCheck(size_t player_id);
    void ClearStatus(size_t player_id);
public slots:
    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();
    void onCallPressed();

    void onLeavePressed();
    void onSettingsPressed();
    void onStartPressed();

private slots:
    void setval();

private:
    void RedrawPlayer(OtherPlayer* player);
    void resizeEvent(QResizeEvent *event);
    void BlockActions();
    void UnBlockActions();

    size_t num_players;

    Player *mPlayer;
    QVector<OtherPlayer*> mOtherPlayers;
    PlayTable *mPlayTable;
    DealerLogo *mDealerLogo;
    Chips *mChips;
    TurnSpark *mTurnIndicator;

    QVector <Card*> CardOnTable;

    QSlider *BetSlider;
    QLineEdit *BetValue;
    WinLabel *mWinLabel;

    QVector<QPushButton*> ActionButtons;
    QPushButton *CallButton;
    QPushButton *BetButton;
    QPushButton *RaiseButton;
    QPushButton *FoldButton;
    QPushButton *CheckButton;

    QPushButton *LeaveButton;
    QPushButton *StartGameButton;
    QPushButton *SettingsButton;
};

#endif // GAMEFRAGMENT_H
