#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screenfactory.h"
#include "Base\navigator.h"

#include <QMainWindow>
#include <thread>
#include <memory>
#include <QStackedWidget>
#include <QUrl>


class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::unique_ptr<std::thread>  client_thread;
    std::unique_ptr<std::thread>  resolver_thread;

    QStackedWidget *container;
    FragmentNavigator *navigator;
    BaseScreensFactory *factory;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
