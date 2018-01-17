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
    slotinputdialog.cpp \
    tool.cpp \
    filechoosedialog.cpp

HEADERS  += mainwindow.h \
    device.h \
    fhwidget.h \
    autoupdate.h \
    slotinputdialog.h \
    tool.h \
    filechoosedialog.h

FORMS    += mainwindow.ui \
    fhwidget.ui \
    autoupdate.ui \
    slotinputdialog.ui \
    filechoosedialog.ui

win32 {
    CONFIG(debug, debug|release) {
#        DEFINES += _DEBUG
        # vld 2.2 downloaded from http://vld.codeplex.com/
#        VLD_PATH = $$quote(C:\Program Files (x86)\Visual Leak Detector)
        INCLUDEPATH += $$quote(C:\Program Files (x86)\Visual Leak Detector\include)
        LIBS += -L$$quote(C:\Program Files (x86)\Visual Leak Detector\lib\Win32) -lvld
    }
}
