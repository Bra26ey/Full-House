#include "mainwindow.h"
#include "screenfactory.h"

#include <QScreen>
#include <QApplication>
#include <QException>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    try {
        qDebug("create main window");
        container = new QStackedWidget;
        container->setObjectName("ui");

        this->factory = new ScreensFactory;
        this->navigator = new FragmentNavigator(this->container, this->factory);

        QString mainStyle = "QWidget#window {"
                            "background-color:#E3F5FC;"
                            "}";
        container->setStyleSheet(mainStyle);
        container->setObjectName("window");

        this->setWindowTitle("Poker House");

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
}
