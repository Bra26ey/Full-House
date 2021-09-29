#ifndef GAMEFRAGMENT_H
#define GAMEFRAGMENT_H

#include "basefragment.h"

#include "playtable.h"
#include "card.h"
#include "dealerlogo.h"
#include "turnspark.h"
#include "winlabel.h"
#include "client_impl.h"
#include "otherplayer.h"

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

public slots: // все ради многопоточки
    void DeletePlayer(size_t player_id);
    void DrawPlayer(size_t player_id, std::string nickname, size_t total_money);

    void SetMinBet(int minbet);
    void SetMaxBet(int maxbet);

    void EndGame(bool is_admin);

    void FlipTableCards();
    void DeleteAllCardsFromTable();
    void AddCardToTable(size_t value,  size_t suit, bool upsided);
    void FlipAllCards();
    void DeleteAllPlayersCards();

    void ShowActions();
    void BlockActions();
    void UnBlockActions();
    void ShowStart();

    void MakeDealer(size_t player_id);
    void DisplayWinner(size_t player_id);
    void CurrentTurn(size_t player_id);
    void GiveCards(size_t player_id, size_t value1, size_t suit1, size_t value2, size_t suit2);
    void FlipCards(size_t player_id);

    void SetBet(size_t player_id, size_t bet);
    void SetFold(size_t player_id);
    void SetCall(size_t player_id);
    void SetRaise(size_t player_id, size_t bet);
    void SetCheck(size_t player_id);
    void SetMoney(size_t player_id, size_t money);
    void ClearStatus(size_t player_id);

    void AvaliableActions(std::vector<bool> buttons);
    void SetMoneyInBank(size_t money);
    void DeleteWinnerDisplay();
    void ClearBank();


private slots:
    void setval();
    void onBetPressed();
    void onRaisePressed();
    void onFoldPressed();
    void onCheckPressed();
    void onCallPressed();

    void onLeavePressed();
    void onSettingsPressed();
    void onStartPressed();
    void onInfoPressed();

private:
    void RedrawPlayer(OtherPlayer* player);
    void resizeEvent(QResizeEvent *event);

    int mMinbet;
    int mMaxbet;

    bool is_active;

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
    QPushButton *ShowRoomInfo;
    QPushButton *StartGameButton;
    QPushButton *SettingsButton;
};

#endif // GAMEFRAGMENT_H
