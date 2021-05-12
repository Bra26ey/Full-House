#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screenfactory.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QUrl>

#include "navigator.h"

#include "client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QStackedWidget *container;
    FragmentNavigator *navigator;
    BaseScreensFactory *factory;

    network::Client client_;

    //some_client_network_implementation

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
