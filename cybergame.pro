#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T08:20:03
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++14


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cybergame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    button.cpp \
    gamechoice.cpp \
    game1.cpp \
    countdown.cpp \
    game2.cpp \
    endgame.cpp \
    memorytile.cpp \
    rules.cpp \
    maintenance.cpp

HEADERS  += mainwindow.h \
    login.h \
    button.h \
    gamechoice.h \
    game1.h \
    countdown.h \
    game2.h \
    endgame.h \
    memorytile.h \
    rules.h \
    maintenance.h

FORMS    += mainwindow.ui
