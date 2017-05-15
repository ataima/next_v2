#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T23:09:37
#
#-------------------------------------------------


macx:QT       += macextras
macx:QMAKE_MAC_SDK = macosx10.9

QT       += core gui widgets



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meLa
TEMPLATE = app



windows{
DEFINES += _STATIC_CPPLIB
}

# DEFINES += _UNICODE

SOURCES += \
../../source/main.cpp\
../../source/mainwindow.cpp\


HEADERS  += \
../../header/mainwindow.h\



DESTDIR = ../../../../dist
windows{
LIBS += $$DESTDIR/n2draw.lib
LIBS += $$DESTDIR/bmpImage.lib
}

linux{
LIBS += $$DESTDIR/libn2draw.a
LIBS += $$DESTDIR/libbmpImage.a
}

macx {
LIBS +=   $$DESTDIR/libn2draw.a
LIBS +=   $$DESTDIR/libbmpImage.a
}

ios {
LIBS +=   $$DESTDIR/libn2draw.a
LIBS +=   $$DESTDIR/libbmpImage.a
}


INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header
INCLUDEPATH +=../../../n2draw/header
linux:!android {
INCLUDEPATH += /usr/include
}

RESOURCES += \
    ../../resource/mela.qrc


linux {
CONFIG += c++14
}

macx{
CONFIG += c++14
}

ios{
CONFIG += c++14
}


INCLUDEPATH -= $$QMAKE_DEFAULT_INCDIRS
