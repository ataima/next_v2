QT += core



TARGET = tests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# DEFINES += _UNICODE

SOURCES += \
        ../../source/mainApp.cpp\
        ../../source/testminiXml.cpp\
        ../../source/testn2imagemanager.cpp\
        ../../source/testappmanager.cpp\
        ../../source/testn2connection.cpp\
        ../../source/testn2objmanager.cpp\
        ../../source/testbmpImage.cpp\
        ../../source/testn2draw.cpp\
        ../../source/testviewglue.cpp



HEADERS += \
        ../../header/CPPtester.h\
        ../../header/docMacro.h


DESTDIR =  ../../../../dist

windows{
LIBS +=   $$DESTDIR/n2draw.lib
LIBS +=   $$DESTDIR/bmpImage.lib
}

linux {
LIBS +=   $$DESTDIR/libn2draw.a
LIBS +=   $$DESTDIR/libbmpImage.a
}

INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header
INCLUDEPATH +=../../../n2draw/header


linux {
CONFIG += c++11
}
