#ifndef PLAYERINFOLABEL_H
#define PLAYERINFOLABEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <iostream>

class PlayerInfoLabel : public QLabel {
public:
    PlayerInfoLabel(std::string nickname, size_t money);
    ~PlayerInfoLabel();
    void setBet(size_t bet);
    void setFold();
    void setRaise(size_t bet);
    void setCheck();
    void setCall();
    void setMoney(size_t money);
    void ClearStatus();

    void setTotal(size_t total);
    size_t GetBet();
    size_t GetTotal();
    QString GetName();

private:
    QLabel *mNickname;
    QLabel *mTotalMoney;
    QLabel *mStatus;

    size_t TotalMoney;
    size_t Bet;
};

#endif // PLAYERINFOLABEL_H
