#-------------------------------------------------
#
# Project created by QtCreator 2013-06-29T11:27:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp \
    fileutils.cpp

HEADERS  += mainwindow.h \
    matrix.h \
    fileutils.h

FORMS    += mainwindow.ui

RESOURCES += \
    spreadsheet.qrc

OTHER_FILES +=
