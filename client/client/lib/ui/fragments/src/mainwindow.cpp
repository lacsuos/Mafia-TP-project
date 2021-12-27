#include "mainwindow.h"
#include "screensfactory.h"
#include "client_impl.h"
#include "resolver.h"

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
        this->navigator = new FragmentNavigator(this->container, this->factory, &resolver);

        PlayerData::reset();

        resolver_thread = std::make_unique<std::thread>([&] { resolver.Run(); });
        resolver_thread->detach();

        this->resize(QApplication::screens().at(0)->availableGeometry().size() * 0.7);
        this->setCentralWidget(container);
    } catch (std::exception& e) {
        qDebug("%s", e.what());
    }
}

MainWindow::~MainWindow() {
    Client->disconnect();
    delete navigator;
    delete container;
    delete factory;
}

#include "moc_mainwindow.cpp"
