#include "fragmentnavigator.h"
#include "screensfactory.h"

using namespace screens;

FragmentNavigator::FragmentNavigator(
        QStackedWidget *container,
        AbstractScreensFactory *screensFactory,
        Resolver* resolver) : mResolver(resolver) {

    this->screensFactory = screensFactory;
    this->currentContainer = container;
    AbstractFragment* startFragment = getStartScreen();

    MainFragment* main = static_cast<MainFragment*>(startFragment);
    //connect(mResolver, &Resolver::DeletePlayer, game, &GameFragment::DeletePlayer, Qt::QueuedConnection);
    connect(mResolver, &Resolver::netError, main, &MainFragment::onNetError, Qt::QueuedConnection);
    connect(mResolver, &Resolver::joined, main, &MainFragment::onJoined, Qt::QueuedConnection);
    connect(mResolver, &Resolver::created, main, &MainFragment::onCreated, Qt::QueuedConnection);
    qDebug("add widget");

    this->stack.push_back(startFragment);
    currentContainer->addWidget(stack.back());
    currentContainer->setCurrentIndex(0);
}

FragmentNavigator::~FragmentNavigator() {
}


void FragmentNavigator::navigateTo(QString tag) {
    qDebug("navigateTo");
    AbstractFragment *newFragment = this->screensFactory->create(tag);
    stack.back()->onPause();
    disconnectFragment(stack.back());
    connectFragment(newFragment);
    stack.push_back(newFragment);
    //connect(mResolver, &Resolver::serverDisconnected, newFragment, &newFragment::disconnect, Qt::QueuedConnection);

    if (tag == CREATING_TAG) {
        CreatingFragment* creating = static_cast<CreatingFragment*>(newFragment);
        //connect(mResolver, &Resolver::DeletePlayer, game, &GameFragment::DeletePlayer, Qt::QueuedConnection);
        connect(mResolver, &Resolver::netError, creating, &CreatingFragment::onNetError, Qt::QueuedConnection);
        connect(mResolver, &Resolver::drawPlayer, creating, &CreatingFragment::onDrawPlayer, Qt::QueuedConnection);
        connect(mResolver, &Resolver::deletePlayer, creating, &CreatingFragment::onDeletePlayer, Qt::QueuedConnection);

    } else if (tag == WAITING_TAG) {
        WaitingFragment* waiting = static_cast<WaitingFragment*>(newFragment);
        connect(mResolver, &Resolver::startGame, waiting, &WaitingFragment::onGameStarts, Qt::QueuedConnection);
    } else if (tag == GAME_TAG) {
        GameFragment* game = static_cast<GameFragment*>(newFragment);
        connect(mResolver, &Resolver::winGame, game, &GameFragment::onWin, Qt::QueuedConnection);
        connect(mResolver, &Resolver::loseGame, game, &GameFragment::onLose, Qt::QueuedConnection);
        connect(mResolver, &Resolver::mafiaWin, game, &GameFragment::onMafiaWin, Qt::QueuedConnection);
        connect(mResolver, &Resolver::citizenWin, game, &GameFragment::onCitizenWin, Qt::QueuedConnection);


    }
    currentContainer->addWidget(newFragment);
    currentContainer->setCurrentWidget(newFragment);

}

AbstractFragment* FragmentNavigator::Front() {
    return stack.front();
}

void FragmentNavigator::back() {
    qDebug("Navigator back");
    currentContainer->removeWidget(stack.back());
    delete stack.back();
    stack.pop_back();
    connectFragment(stack.back());
    stack.back()->onResume();
    currentContainer->setCurrentWidget(stack.back());
}



void FragmentNavigator::newRootScreen(QString tag) {
    qDebug("Navigator newRootScreen");
    AbstractFragment *newFragment = this->screensFactory->create(tag);
    disconnectFragment(stack.back());
    stack.clear();
    connectFragment(newFragment);
    for(int i = currentContainer->count(); i >= 0; i--) {
        QWidget* widget = currentContainer->widget(i);
        currentContainer->removeWidget(widget);
        widget->deleteLater();
    }
    currentContainer->addWidget(newFragment);
    stack.push_back(newFragment);
}


AbstractFragment* FragmentNavigator::getStartScreen() {
    return createAndConnect(this->screensFactory->createStart());
}


void FragmentNavigator::connectFragment(AbstractFragment *fragment) {
    qDebug("Navigator connect slots");
    connect(fragment, &AbstractFragment::back, this, &FragmentNavigator::back);
    connect(fragment, &AbstractFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    connect(fragment, &AbstractFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    connect(fragment, &AbstractFragment::Front, this, &FragmentNavigator::Front);
}

void FragmentNavigator::disconnectFragment(AbstractFragment *fragment) {
    qDebug("Navigator disconnect slots");
    disconnect(fragment, &AbstractFragment::back, this, &FragmentNavigator::back);
    disconnect(fragment, &AbstractFragment::navigateTo, this, &FragmentNavigator::navigateTo);
    disconnect(fragment, &AbstractFragment::newRootScreen, this, &FragmentNavigator::newRootScreen);
    disconnect(fragment, &AbstractFragment::Front, this, &FragmentNavigator::Front);
}

AbstractFragment* FragmentNavigator::createAndConnect(QString tag) {
    qDebug("Navigator create screen");
    AbstractFragment *fragment = this->screensFactory->create(tag);
    connectFragment(fragment);
    return fragment;
}

#include "moc_fragmentnavigator.cpp"
