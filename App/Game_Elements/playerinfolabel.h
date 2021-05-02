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

private:
    QLabel *mNickname;
    QLabel *mTotalMoney;
    QLabel *mBet;
};

#endif // PLAYERINFOLABEL_H
