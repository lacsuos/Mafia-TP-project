#include "fragmentnavigator.h"
#include "screensfactory.h"

using namespace screens;

FragmentNavigator::FragmentNavigator(
        QStackedWidget *container,
        AbstractScreensFactory *screensFactory) {

    this->screensFactory = screensFactory;
    this->currentContainer = container;
    AbstractFragment* startFragment = getStartScreen();

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
