#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screensfactory.h"
#include "fragmentnavigator.h"

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
    AbstractScreensFactory *factory;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
