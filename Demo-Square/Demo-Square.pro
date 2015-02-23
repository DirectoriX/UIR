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
    ../GenClasses.cpp

HEADERS += square.h \
    ../GenClasses.h

DISTFILES += Demo-Square.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
