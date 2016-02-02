#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T21:02:29
#
#-------------------------------------------------

QT       -= gui

macx:QT       += macextras
macx:QMAKE_MAC_SDK = macosx10.9

TARGET = image
TEMPLATE = lib
CONFIG += staticlib

DEFINES += BMPIMAGE_LIBRARY
# DEFINES += _UNICODE

windows{
DEFINES += _STATIC_CPPLIB
}

SOURCES += ../../source/images.cpp

HEADERS += ../../header/images.h

DESTDIR = ../../../../dist

TARGET = bmpImage

INCLUDEPATH +=../../header

linux {
CONFIG += c++11
}

macx{
CONFIG += c++11
}

ios{
CONFIG += c++11
}
