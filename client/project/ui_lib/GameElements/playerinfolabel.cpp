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
    mNickname->setGeometry(0, 50, 300, 30);
    mNickname->setStyleSheet("background:rgba(33,33,33,0.5);font-size:21px;color:rgb(242, 224, 22)");
    mNickname->setAlignment(Qt::AlignCenter);

    mTotalMoney->setParent(this);
    mTotalMoney->setGeometry(0, 80, 300, 30);
    mTotalMoney->setStyleSheet("background:rgba(33,33,33,0.5);font-size:21px;color:rgb(242, 224, 22)");
    mTotalMoney->setAlignment(Qt::AlignCenter);

    mStatus->setParent(this);
    mStatus->setGeometry(0, 110, 300, 30);
    mStatus->setStyleSheet("background:rgba(33,33,33,0.5);font-size:21px;color:rgb(0, 255, 255);");
    mStatus->setAlignment(Qt::AlignCenter);
}

void PlayerInfoLabel::setTotal(size_t total) {
    TotalMoney = total;
    QString totalm = "Bank 💰: " + QString::number(TotalMoney);
    mTotalMoney->setText(totalm);
}

void PlayerInfoLabel::setBet(size_t bet) {
    QString total = "Bet 🌝: " + QString::number(bet);
    mStatus->setText(total);
}

void PlayerInfoLabel::setMoney(size_t money) {
    QString totalm = "Bank 💰: " + QString::number(money);
    mTotalMoney->setText(totalm);
}

void PlayerInfoLabel::setRaise(size_t bet) {
    QString total = "Raise 🌝: " + QString::number(bet);
    mStatus->setText(total);
}

void PlayerInfoLabel::setFold() {
    QString status = "Fold ❎";
    mStatus->setText(status);
}

void PlayerInfoLabel::setCheck() {
    QString status = "Check ✅";
    mStatus->setText(status);
}

void PlayerInfoLabel::setCall() {
    QString status = "Call";
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

QString PlayerInfoLabel::GetName() {
    return mNickname->text();
}
