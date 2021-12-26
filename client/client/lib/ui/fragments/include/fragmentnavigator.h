   #ifndef FRAGMENTNAVIGATOR_H
#define FRAGMENTNAVIGATOR_H

#include "abstractfragment.h"
#include "abstractscreensfactory.h"
#include "resolver.h"

#include <QStackedWidget>
#include <list>

class FragmentNavigator: public QObject {
    Q_OBJECT

public:
    FragmentNavigator() = delete;
    FragmentNavigator(QStackedWidget *container, AbstractScreensFactory *screensFactory, Resolver* resolver);
    ~FragmentNavigator();
    AbstractFragment* getStartScreen();


public slots:
    void navigateTo(QString tag);

    void back();

    void newRootScreen(QString tag);

    AbstractFragment* Front();

private:
    QStackedWidget *currentContainer;

    AbstractScreensFactory *screensFactory;
    std::list<AbstractFragment*> stack;

    AbstractFragment* createAndConnect(QString tag);

    void connectFragment(AbstractFragment *fragment);
    void disconnectFragment(AbstractFragment *fragment);

    Resolver* mResolver;
};

#endif // FRAGMENTNAVIGATOR_H
