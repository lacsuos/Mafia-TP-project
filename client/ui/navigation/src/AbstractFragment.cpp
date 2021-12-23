#include "AbstractFragment.h"
#include <QLayout>

AbstractFragment::AbstractFragment(){}
AbstractFragment::~AbstractFragment(){}
void AbstractFragment::onPause(){}
void AbstractFragment::onResume(){}

void AbstractFragment::clearList(Qlayot* list){
    QLayoutItem* item;
    while(list->count() != 0){
        item = list->takeAt(0);
        if (item->layout() != 0){
            clearList(item->layout())
        }
        else if(item->widget() != 0){
            delete item->widget();
        }
        delete item;
    }
}