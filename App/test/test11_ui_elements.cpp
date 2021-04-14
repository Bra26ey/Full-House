#include "gtest/gtest.h"

#include "gotoregistrationbutton.h"
#include "gotologinbutton.h"
#include "gotomenubutton.h"
#include "gotosearchbutton.h"
#include "gotosettingsbutton.h"
#include "mutebutton.h"
#include "loginbutton.h"
#include "registrationbutton.h"

#include "betbutton.h"
#include "foldbutton.h"
#include "raisebutton.h"
#include "checkbutton.h"




#include <QApplication>

TEST(buttons, test1) {
    int argc = 1;
    char *argv[0];

    QApplication a(argc, argv);

    ASSERT_NO_THROW(GoToLoginButton btn);
    ASSERT_NO_THROW(GoToRegistrationButton btn);
    ASSERT_NO_THROW(GoToMenuButton btn);
    ASSERT_NO_THROW(GoToSearchButton btn);
    ASSERT_NO_THROW(GoToSettingsButton btn);
    ASSERT_NO_THROW(MuteButton btn);
    ASSERT_NO_THROW(LoginButton btn);
    ASSERT_NO_THROW(RegistrationButton btn);

    // GamePlayButtons
    ASSERT_NO_THROW(RaiseButton btn);
    ASSERT_NO_THROW(BetButton btn);
    ASSERT_NO_THROW(FoldButton btn);
    ASSERT_NO_THROW(CheckButton btn);


    auto btn1 = new GoToLoginButton;
    ASSERT_NO_THROW(btn1->show());
    ASSERT_NO_THROW(btn1->clicked());
    delete btn1;

    auto btn2 = new GoToRegistrationButton;
    ASSERT_NO_THROW(btn2->show());
    ASSERT_NO_THROW(btn2->clicked());
    delete btn2;

    auto btn3 = new GoToMenuButton;
    ASSERT_NO_THROW(btn3->show());
    ASSERT_NO_THROW(btn3->clicked());
    delete btn3;

    auto btn4 = new GoToSearchButton;
    ASSERT_NO_THROW(btn4->show());
    ASSERT_NO_THROW(btn4->clicked());
    delete btn4;

    auto btn5 = new GoToSettingsButton;
    ASSERT_NO_THROW(btn5->show());
    ASSERT_NO_THROW(btn5->clicked());
    delete btn5;

    auto btn6 = new MuteButton;
    ASSERT_NO_THROW(btn6->show());
    ASSERT_NO_THROW(btn6->clicked());
    delete btn6;

    auto btn7 = new LoginButton;
    ASSERT_NO_THROW(btn7->show());
    ASSERT_NO_THROW(btn7->clicked());
    delete btn7;

    auto btn8 = new RegistrationButton;
    ASSERT_NO_THROW(btn8->show());
    ASSERT_NO_THROW(btn8->clicked());
    delete btn8;

    auto btn9 = new BetButton;
    ASSERT_NO_THROW(btn9->show());
    ASSERT_NO_THROW(btn9->clicked());
    delete btn9;

    auto btn10 = new RaiseButton;
    ASSERT_NO_THROW(btn10->show());
    ASSERT_NO_THROW(btn10->clicked());
    delete btn10;

    auto btn11 = new FoldButton;
    ASSERT_NO_THROW(btn11->show());
    ASSERT_NO_THROW(btn11->clicked());
    delete btn11;

    auto btn12 = new CheckButton;
    ASSERT_NO_THROW(btn12->show());
    ASSERT_NO_THROW(btn12->clicked());
    delete btn12;

}

