QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = remote-assistance
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    recoursewindow.cpp \
    assistwindow.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    mainwindows.h \
    recoursewindow.h \
    assistwindow.h
