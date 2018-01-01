#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T10:37:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FHSIP_v2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    device.cpp \
    fhwidget.cpp \
    autoupdate.cpp \
    slotinputdialog.cpp

HEADERS  += mainwindow.h \
    device.h \
    fhwidget.h \
    autoupdate.h \
    slotinputdialog.h

FORMS    += mainwindow.ui \
    fhwidget.ui \
    autoupdate.ui \
    slotinputdialog.ui
