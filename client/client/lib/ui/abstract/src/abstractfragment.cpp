#include "abstractfragment.h"
#include <QVBoxLayout>

AbstractFragment::AbstractFragment() {}
AbstractFragment::~AbstractFragment() {}
void AbstractFragment::onPause() {}
void AbstractFragment::onResume() {}

void AbstractFragment::clearList(QLayout *list) {
    while(list->count()!=0) {
        QLayoutItem* child;
        child = list->takeAt(0);
        if(child->layout() != 0)
        {
            clearList(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }
        delete child;
    }
}

#include "moc_abstractfragment.cpp"
