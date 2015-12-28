#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T23:09:37
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meLa
TEMPLATE = app
DEFINES += _UNICODE

SOURCES += \
../../source/main.cpp\
../../source/mainwindow.cpp\
../../source/mdichild.cpp \
../../source/mdiselector.cpp \

HEADERS  += \
../../header/mainwindow.h\
../../header/mdichild.h\
../../header/mdiselector.h\



DESTDIR = ../../../../../../dist

LIBS += ../../../../../../dist/n2draw.lib
LIBS += ../../../../../../dist/bmpImage.lib




INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header
INCLUDEPATH +=../../../n2draw/header

RESOURCES += \
    ../../resource/mela.qrc
