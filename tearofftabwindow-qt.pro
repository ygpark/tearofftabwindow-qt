#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T19:08:41
#
#-------------------------------------------------

QT       += core gui

TARGET = tearofftabwindow-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cqtabwidget.cpp \
    cqtabbar.cpp \
    form.cpp \
    cwindowmanager.cpp

HEADERS  += mainwindow.h \
    cqtabwidget.h \
    cqtabbar.h \
    form.h \
    cwindowmanager.h

FORMS    += mainwindow.ui \
    form.ui

RESOURCES += \
    qtabwidget.qrc

OTHER_FILES += \
    css/cqtabwidget.css
