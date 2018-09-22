#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T20:31:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Y86
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    Decode.cpp \
    Execute.cpp \
    Fetch.cpp \
    Memory.cpp \
    translation.cpp \
    Writeback.cpp

HEADERS  += mainwindow.h \
    controller.h \
    Decode.h \
    Execute.h \
    Fetch.h \
    Memory.h \
    translation.h \
    Writeback.h

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11
RC_ICONS = cpu.ico
