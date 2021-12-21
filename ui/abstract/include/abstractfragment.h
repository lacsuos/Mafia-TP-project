    #ifndef ABSTRACTFRAGMENT_H
#define ABSTRACTFRAGMENT_H

#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

class AbstractFragment: public QFrame {
    Q_OBJECT

signals:
    void back();

    void navigateTo(QString tag);

    void newRootScreen(QString tag);
    void replace(QString tag);

    AbstractFragment* Front();

protected:
    void clearList(QLayout *list);

public:
    AbstractFragment();
    ~AbstractFragment();
    virtual void onPause();
    virtual void onResume();
};

#endif // ABSTRACTFRAGMENT_H
