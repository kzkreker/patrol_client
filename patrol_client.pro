#-------------------------------------------------
#
# Project created by QtCreator 2013-03-30T17:08:11
#
#-------------------------------------------------

QT       += core gui network sql xml
CONFIG += serialport  console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = patrol_client

TEMPLATE = app

INCLUDEPATH += . ../libmaia/
LIBS += ../libmaia/libmaia.a
LIBS     += -lmarblewidget -lQtGui -lQtCore


DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp\
           mainwindow.cpp \
           database.cpp \
           mypaintlayer.cpp \
           client.cpp

HEADERS  += mainwindow.h \
            relay.h \
            GPS.h \
            database.h \
            mypaintlayer.h \
            client.h \
    picweb.h \
    hardware.h

FORMS    += mainwindow.ui


