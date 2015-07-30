#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T05:50:58
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Demo-Square
TEMPLATE = lib
CONFIG += plugin

SOURCES += square.cpp \
    ../rng.cpp \
    infowindow.cpp

HEADERS += square.h \
    ../icreature.h \
    ../rng.h \
    infowindow.h

DISTFILES += Demo-Square.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    infowindow.ui
