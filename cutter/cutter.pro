#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T16:22:43
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += plugin

TARGET = cutter
TEMPLATE = lib

SOURCES += cutter.cpp \
    info.cpp \
    ../rng.cpp \
    drawfield.cpp \
    setup.cpp

HEADERS += cutter.h \
    info.h \
    ../icreature.h \
    ../rng.h \
    drawfield.h \
    descr.h \
    setup.h
DISTFILES += cutter.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    info.ui \
    setup.ui

CONFIG += c++11
