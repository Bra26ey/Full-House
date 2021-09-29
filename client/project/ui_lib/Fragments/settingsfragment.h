#ifndef SETTINGSFRAGMENT_H
#define SETTINGSFRAGMENT_H

#include "basefragment.h"

#include <QSlider>
#include <QLabel>
#include <QPushButton>

class SettingsFragment: public BaseFragment {
    Q_OBJECT
public:
    SettingsFragment();
    ~SettingsFragment();

public slots:
    void MutePressed();
    void ChangedSlider();
    void onMenuPressed();

private:
    QLabel* Nickname;
    QLabel* Money;
    QPushButton *MuteButton;
    QPushButton *MenuButton;
    QSlider *VolumeSlider;
    size_t EasterEgg;
};

#endif // SETTINGSFRAGMENT_H
