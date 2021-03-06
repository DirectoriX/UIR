#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T05:49:15
#
#-------------------------------------------------

QT       += concurrent core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qwt

TARGET = GeneticSim
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    ../rng.cpp \
    tpopulation.cpp

TRANSLATIONS += GeneticSim_ru.ts

HEADERS += mainwindow.h \
    ../icreature.h \
    ../rng.h \
    tpopulation.h

FORMS += mainwindow.ui

RESOURCES += \
    res.qrc

RC_ICONS = main.ico

CONFIG += c++11
