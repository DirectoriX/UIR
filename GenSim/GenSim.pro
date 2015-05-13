#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T05:49:15
#
#-------------------------------------------------

QT       += concurrent core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GenSim
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    ../GenClasses.cpp

HEADERS  += mainwindow.h \
    ../GenClasses.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
