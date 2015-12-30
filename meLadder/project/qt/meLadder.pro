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



DESTDIR = ../../../../dist
windows{
LIBS += $$DESTDIR/n2draw.lib
LIBS += $$DESTDIR/bmpImage.lib
}

linux{
LIBS += $$DESTDIR/libn2draw.a
LIBS += $$DESTDIR/libbmpImage.a
}


INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header
INCLUDEPATH +=../../../n2draw/header
linux {
INCLUDEPATH += /usr/include
}

RESOURCES += \
    ../../resource/mela.qrc


linux {
CONFIG += c++11
}
