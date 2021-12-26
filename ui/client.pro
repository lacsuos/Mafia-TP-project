QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstract/src/abstractfragment.cpp \
    abstract/src/abstractscreensfactory.cpp \
    abstract/src/fragmentnavigator.cpp \
    client/src/asyncgameclientconnector.cpp \
    client/src/gameclient.cpp \
    client/src/igameclient.cpp \
    client/src/igameclientlistener.cpp \
    fragments/src/creatingfragment.cpp \
    fragments/src/gamefragment.cpp \
    fragments/src/mainfragment.cpp \
    fragments/src/optionsfragment.cpp \
    fragments/src/waitingfragment.cpp \
    main.cpp \
    main/src/mainwindow.cpp \
    main/src/screensfactory.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    abstract/include/abstractfragment.h \
    abstract/include/abstractscreensfactory.h \
    abstract/include/fragmentnavigator.h \
    client/include/asyncgameclientconnector.h \
    client/include/gameclient.h \
    client/include/igameclient.h \
    client/include/igameclientlistener.h \
    fragments/include/creatingfragment.h \
    fragments/include/gamefragment.h \
    fragments/include/mainfragment.h \
    fragments/include/optionsfragment.h \
    fragments/include/waitingfragment.h \
    main/include/mainwindow.h \
    main/include/screensfactory.h \
