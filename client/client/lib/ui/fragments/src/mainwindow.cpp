#include "mainwindow.h"
#include "screensfactory.h"

#include <QScreen>
#include <QApplication>
#include <QException>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    try {
        this->setWindowTitle("Mafia by MathGames");

        container = new QStackedWidget;
        container->setObjectName("GUI");

        QString mainStyle = "QWidget#window {"
                            "background-color:rgb(243, 243, 211);"
                            "}";
        container->setStyleSheet(mainStyle);
        container->setObjectName("window");

        this->factory = new ScreensFactory;
        this->navigator = new FragmentNavigator(this->container, this->factory);

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

#include "moc_mainwindow.cpp"
