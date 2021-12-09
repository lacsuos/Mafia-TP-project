QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/basefragment.cpp \
    base/basescreensfactory.cpp \
    base/navigator.cpp \
    fragments/gamefragment.cpp \
    fragments/mainfragment.cpp \
    fragments/optionsfragment.cpp \
    main.cpp \
    mainwindow.cpp \
    screenfactory.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    base/basefragment.h \
    base/basescreensfactory.h \
    base/navigator.h \
    fragments/gamefragment.h \
    fragments/mainfragment.h \
    fragments/optionsfragment.h \
    mainwindow.h \
    screenfactory.h \
