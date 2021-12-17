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
    fragments/src/gamefragment.cpp \
    fragments/src/mainfragment.cpp \
    fragments/src/optionsfragment.cpp \
    main.cpp \
    main/scr/mainwindow.cpp \
    main/scr/screensfactory.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    abstract/include/abstractfragment.h \
    abstract/include/abstractscreensfactory.h \
    abstract/include/fragmentnavigator.h \
    fragments/include/gamefragment.h \
    fragments/include/mainfragment.h \
    fragments/include/optionsfragment.h \
    main/include/mainwindow.h \
    main/include/screensfactory.h \
