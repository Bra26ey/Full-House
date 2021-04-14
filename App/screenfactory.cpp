#include "screenfactory.h"
#include "loginfragment.h"
#include "registrationfragment.h"


using namespace screens;

ScreensFactory::ScreensFactory() {

}

ScreensFactory::~ScreensFactory() {

}

BaseFragment* ScreensFactory::create(QString tag) {
    qDebug("ScreensFactory create");

    if (tag == LOGIN_TAG) {
        return new LoginFragment;
    }
    if (tag == REGISTRATION_TAG) {
        return new RegistrationFragment;
    } else {
        return nullptr;
    }
}

QString ScreensFactory::createStart() {
    return LOGIN_TAG;
}
