#pragma once

#include <QFrame>
#include <QString>
#include <QLayout>  

class AbstractFragment : public QFrame{
    Q_OBJECT

signals:
    void back();
    void moveTo(QString flag);
    void newRootScreem(QString flag);
    void replace(QString flag);
protected:
    void clearList(QLayout* list);
public:
    AbstractFragment();
    ~AbstractFragment();

    virtual void onPause();
    virtual void onResume();
};