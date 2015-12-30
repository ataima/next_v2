#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T21:02:29
#
#-------------------------------------------------

QT       -= gui

TARGET = image
TEMPLATE = lib
CONFIG += staticlib

DEFINES += BMPIMAGE_LIBRARY
DEFINES += _UNICODE

SOURCES += ../../source/images.cpp

HEADERS += ../../header/images.h

DESTDIR = ../../../../../../dist

TARGET = bmpImage

INCLUDEPATH +=../../header

linux {
CONFIG += c++11
}
