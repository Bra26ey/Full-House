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
    void setTotal(size_t total);
    size_t GetBet();
    size_t GetTotal();

private:
    QLabel *mNickname;
    QLabel *mTotalMoney;
    QLabel *mBet;

    size_t TotalMoney;
    size_t Bet;
};

#endif // PLAYERINFOLABEL_H
