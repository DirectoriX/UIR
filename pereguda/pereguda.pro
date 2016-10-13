#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T22:11:15
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = pereguda
TEMPLATE = lib
CONFIG += plugin

SOURCES += prg.cpp \
    ../rng.cpp \
    www.cpp

HEADERS += prg.h \
    ../rng.h \
    www.h

DISTFILES += pereguda.json

FORMS += \
    www.ui
