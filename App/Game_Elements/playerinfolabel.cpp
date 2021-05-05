#include "playerinfolabel.h"

PlayerInfoLabel::PlayerInfoLabel(std::string nickname, size_t money) : TotalMoney(money), Bet(0) {
    mNickname = new QLabel;
    mTotalMoney = new QLabel;
    mStatus = new QLabel;

    QString name = QString::fromStdString(nickname);
    mNickname->setText(name);
    QString total = "Bank 💰: " + QString::number(money);
    mTotalMoney->setText(total);

    mNickname->setParent(this);
    mNickname->setGeometry(0, 50, 400, 30);
    mNickname->setStyleSheet("background:rgba(33,33,33,0.5);font-size:24px;color:rgb(242, 224, 22)");

    mTotalMoney->setParent(this);
    mTotalMoney->setGeometry(0, 80, 400, 30);
    mTotalMoney->setStyleSheet("background:rgba(33,33,33,0.5);font-size:24px;color:rgb(242, 224, 22)");

    mStatus->setParent(this);
    mStatus->setGeometry(0, 110, 400, 30);
    mStatus->setStyleSheet("background:rgba(33,33,33,0.5);font-size:24px;color:rgb(0, 255, 255);");
    qDebug("All added");
}

void PlayerInfoLabel::setTotal(size_t total) {
    TotalMoney = total;
    QString totalm = "Bank 💰: " + QString::number(TotalMoney);
    mTotalMoney->setText(totalm);
}

void PlayerInfoLabel::setBet(size_t bet) {
    QString total = "Bet 🌝: " + QString::number(bet);
    Bet = bet;
    TotalMoney -= Bet;
    QString totalm = "Bank 💰: " + QString::number(TotalMoney);
    mTotalMoney->setText(totalm);
    mStatus->setText(total);
}

void PlayerInfoLabel::setRaise() {
    QString status = "Raise 📈";
    mStatus->setText(status);
}

void PlayerInfoLabel::setFold() {
    QString status = "Fold ❎";
    mStatus->setText(status);
}

void PlayerInfoLabel::setCheck() {
    QString status = "Check ✅";
    mStatus->setText(status);
}

void PlayerInfoLabel::ClearStatus() {
    mStatus->setText("");
}

PlayerInfoLabel::~PlayerInfoLabel() {
    delete mNickname;
    delete mTotalMoney;
    delete mStatus;
}

size_t PlayerInfoLabel::GetBet() {
    return Bet;
}

size_t PlayerInfoLabel::GetTotal() {
    return TotalMoney;
}
