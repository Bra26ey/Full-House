#ifndef SETTINGSFRAGMENT_H
#define SETTINGSFRAGMENT_H

#include "basefragment.h"

#include <QSlider>
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
    QPushButton *MuteButton;
    QPushButton *MenuButton;
    QSlider *VolumeSlider;
};

#endif // SETTINGSFRAGMENT_H
