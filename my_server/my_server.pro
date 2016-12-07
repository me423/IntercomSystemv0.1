QT += core
QT -= gui
QT+=widgets
QT+=network

TARGET = my_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    myserver.cpp

HEADERS += \
    myserver.h

