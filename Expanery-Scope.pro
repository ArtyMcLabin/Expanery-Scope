

#-------------------------------------------------
#
# Project created by QtCreator 2016-09-4T13:35:59
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++1z

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Expanery-Scope
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
        cell.cpp \
        zone.cpp \
        engine.cpp \
        food.cpp \
        zoneobject.cpp

HEADERS  += widget.h \
        cell.h \
        zone.h \
        engine.h \
        food.h \
        zoneobject.h

FORMS    += widget.ui
