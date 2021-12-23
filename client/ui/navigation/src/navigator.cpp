#include "navigator.h"

using namespace screen;

Navigator::Navigator(QStackedWidget* container, AbstractScreenFactory* _screenFactory){
    screenFactory = _screenFactory;
    current = container;
    AbstractFragment* start = createStart();
    stack.append(start);
    current->addWidget(stack.last());
    current->setCurrentIndex(0); 
}

Navigator::~Navigator(){}

void Navigator::moveTo(QString flag){
    AbstractFragment* newFragment = screenFactory->createFragment(flag);
    stack.last()->onPause();
    disconnectFragment(stack.last());
    connectFragment(newFragment);
    stack.append(newFragment);
    current->addWidget(newFragment);
    current->setCurrentWidget(newFragment);
}

void Navigator::back(){
    current->removeWidget(stack.last());
    delete stack.last();
    stack.removeLast();
    connectFragment(stack.last());
    stack.last()->onResume();
    current->setCurrentWidget(stack.last());
}