#include "mainwindow.h"
#include "screenfactory.h"

#include <QScreen>
#include <QApplication>
#include <QException>
#include <QDir>
#include "mediaplayer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    try {
        qDebug("create main window");
        container = new QStackedWidget;
        container->setObjectName("ui");

        this->factory = new ScreensFactory;
        this->navigator = new FragmentNavigator(this->container, this->factory);

        QString mainStyle = "QWidget#window {"
                            "background-color:rgb(89, 176, 76);"
                            "}";
        container->setStyleSheet(mainStyle);
        container->setObjectName("window");

        this->setWindowTitle("Full House");

        player = new QMediaPlayer;
        playlist = new QMediaPlaylist();
        playlist->addMedia(QUrl("qrc:/music/Media/BackGround.mp3"));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        player->setPlaylist(playlist);
        player->setVolume(20);
        player->play();


        this->resize(QApplication::screens().at(0)->availableGeometry().size() * 0.7);
        this->setCentralWidget(container);
    } catch (std::exception& e) {
        qDebug("%s", e.what());
    }
}

MainWindow::~MainWindow() {
    delete navigator;
    delete container;
    delete factory;
    delete player;
    delete playlist;
}
