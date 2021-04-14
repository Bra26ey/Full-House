#include "gtest/gtest.h"

#include <QApplication>

#include "volumeslider.h"
#include "betslider.h"

#include "logintextbox.h"
#include "passwordtextbox.h"
#include "emailtextbox.h"

#include "roomidtextbox.h"
#include "roompasswordtextbox.h"

TEST(sliders, test1) {
    int argc = 1;
    char *argv[0];

    QApplication a(argc, argv);

    ASSERT_NO_THROW(BetSlider a);
    ASSERT_NO_THROW(VolumeSlider a);

    auto sl = new BetSlider;
    ASSERT_NO_THROW(sl->show());
    ASSERT_NO_THROW(sl->setTickInterval(1));
    ASSERT_NO_THROW(sl->setTickPosition(QSlider::TicksLeft));
    delete sl;

    auto sl1 = new VolumeSlider;
    ASSERT_NO_THROW(sl1->show());
    ASSERT_NO_THROW(sl1->setTickInterval(1));
    ASSERT_NO_THROW(sl1->setTickPosition(QSlider::TicksLeft));
    delete sl1;
}


TEST(textboxes, test1) {

    int argc = 1;
    char *argv[0];

    QApplication a(argc, argv);

    ASSERT_NO_THROW(LoginTextBox a);
    ASSERT_NO_THROW(PasswordTextBox a);
    ASSERT_NO_THROW(EmailTextBox a);

    ASSERT_NO_THROW(RoomIdTextBox a);
    ASSERT_NO_THROW(RoomPasswordTextBox a);

    auto txt1 = new LoginTextBox;
    ASSERT_NO_THROW(txt1->show());
    ASSERT_NO_THROW(TestBody())


}
