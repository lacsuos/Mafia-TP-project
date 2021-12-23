#include "AbstractScreenFactory.h"

AbstractScreenFactory::AbstractScreenFactory(){}
AbstractScreenFactory::~AbstractScreenFactory(){}
AbstractFragment* AbstractScreenFactory::createFragment(QString flag){}
QString AbstractScreenFactory::createStart(){}