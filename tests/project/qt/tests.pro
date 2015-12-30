QT += core

TARGET = tests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


DEFINES += _UNICODE

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


DESTDIR =  ../../../../../../dist

LIBS +=  ../../../../../../dist/n2draw.lib
LIBS +=  ../../../../../../dist/bmpImage.lib




INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header
INCLUDEPATH +=../../../n2draw/header


linux {
CONFIG += c++11
}
