#include "mainwindow.h"
#include "screenfactory.h"

#include <QScreen>
#include <QApplication>
#include <QException>
#include <QThread>
#include <QDir>
#include "mediaplayer.h"
#include "client_impl.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    try {
        container = new QStackedWidget;
        container->setObjectName("ui");

        this->factory = new ScreensFactory;
        this->navigator = new FragmentNavigator(this->container, this->factory, &resolver);

        QString mainStyle = "QWidget#window {"
                            "background-color:rgb(47, 140, 140);"
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
         // player->play();

        Client = std::make_unique<network::Client>();
        Client->Connect();
        client_thread = std::make_unique<std::thread>([&] { Client->Run(); });
        client_thread->detach();

        resolver_thread = std::make_unique<std::thread>([&] { resolver.Run(); });
        resolver_thread->detach();

        this->resize(QApplication::screens().at(0)->availableGeometry().size() * 0.7);
        this->setCentralWidget(container);
    } catch (std::exception& e) {
        qDebug("%s", e.what());
    }
}

MainWindow::~MainWindow() {
    Client->Disconnect();
    delete navigator;
    delete container;
    delete factory;
    delete player;
    delete playlist;
}
