#ifndef PLAYERINFOLABEL_H
#define PLAYERINFOLABEL_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

class PlayerInfoLabel : public QWidget {
public:
    PlayerInfoLabel();
    ~PlayerInfoLabel();

private:
    QPixmap *mAvatar;
    QLabel *mNickname;
};

#endif // PLAYERINFOLABEL_H
