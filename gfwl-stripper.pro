#-------------------------------------------------
#
# Project created by QtCreator 2013-10-18T10:44:15
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS +=  -s -Wall -ansi -pedantic -std=c++0x -Werror

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GFWL-stripper
TEMPLATE = app
RC_FILE = nogfwl.rc

SOURCES +=  main.cpp\
            mainwindow.cpp\
            gamefwl.cpp

HEADERS  += mainwindow.h\
            gamefwl.h

FORMS    += mainwindow.ui

ICON    = nogfwl.ico
OTHER_FILES = Games.ini

DEPLOYMENT += OTHER_FILES
