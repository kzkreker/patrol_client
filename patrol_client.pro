#-------------------------------------------------
#
# Project created by QtCreator 2013-03-30T17:08:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = patrol_client

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS     += -lmarblewidget -lQtGui -lQtCore
