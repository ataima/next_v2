#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T19:26:00
#
#-------------------------------------------------

QT       -= gui


macx:QT       += macextras
macx:QMAKE_MAC_SDK = macosx10.9

TARGET = n2draw
TEMPLATE = lib
CONFIG += staticlib

DEFINES += N2DRAW_LIBRARY

#DEFINES += _UNICODE


windows{
DEFINES += _STATIC_CPPLIB
}

SOURCES += \
../../source/n2appmanager.cpp            \
../../source/n2draw.cpp                  \
../../source/n2imagemanager.cpp          \
../../source/n2view.cpp                  \
../../source/n2connection.cpp            \
../../source/n2drawmanager.cpp           \
../../source/n2miniXml.cpp               \
../../source/n2viewglue.cpp              \
../../source/n2commander.cpp             \
../../source/n2toolview.cpp              \
../../source/n2exthandler.cpp            \
../../source/n2fontlist.cpp              \
../../source/n2fontmanager.cpp           \
../../source/n2selector.cpp              \
../../source/n2childapp.cpp              \
../../source/n2scroller.cpp              \
../../source/n2caption.cpp               \
../../source/n2logger.cpp                \
../../source/n2utils.cpp                 \
../../source/n2moreinfo.cpp              \
../../source/n2capturepos.cpp \
    ../../source/n2resource.cpp



HEADERS += \
../../header/MerlinoVCPU.h             \
../../header/n2point.h                 \
../../header/n2connection.h            \
../../header/n2drawmanager.h           \
../../header/n2miniXml.h               \
../../header/n2viewglue.h              \
../../header/n2appmanager.h            \
../../header/n2draw.h                  \
../../header/n2imagemanager.h          \
../../header/n2view.h                  \
../../header/n2utoatou.h               \
../../header/n2exception.h             \
../../header/n2commander.h             \
../../header/n2toolview.h              \
../../header/n2interfaces.h            \
../../header/n2exthandler.h            \
../../header/n2fontlist.h              \
../../header/n2fontmanager.h           \
../../header/n2selector.h              \
../../header/n2childapp.h              \
../../header/n2scroller.h              \
../../header/n2caption.h               \
../../header/n2logger.h                \
../../header/n2utils.h                 \
../../header/n2capturepos.h \
    ../../header/n2resource.h


DESTDIR =../../../../dist

TARGET = n2draw




INCLUDEPATH +=../../header
INCLUDEPATH +=../../../bmpImage/header


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

DISTFILES += \
    ../../../resource/conf_utf16.xml \
    ../../../resource/conf_utf8.xml \
    ../../../resource/old/Courier_10_14/100_10_14.bmp \
    ../../../resource/old/Courier_10_14/101_10_14.bmp \
    ../../../resource/old/Courier_10_14/102_10_14.bmp \
    ../../../resource/old/Courier_10_14/103_10_14.bmp \
    ../../../resource/old/Courier_10_14/104_10_14.bmp \
    ../../../resource/old/Courier_10_14/105_10_14.bmp \
    ../../../resource/old/Courier_10_14/106_10_14.bmp \
    ../../../resource/old/Courier_10_14/107_10_14.bmp \
    ../../../resource/old/Courier_10_14/108_10_14.bmp \
    ../../../resource/old/Courier_10_14/109_10_14.bmp \
    ../../../resource/old/Courier_10_14/110_10_14.bmp \
    ../../../resource/old/Courier_10_14/111_10_14.bmp \
    ../../../resource/old/Courier_10_14/112_10_14.bmp \
    ../../../resource/old/Courier_10_14/113_10_14.bmp \
    ../../../resource/old/Courier_10_14/114_10_14.bmp \
    ../../../resource/old/Courier_10_14/115_10_14.bmp \
    ../../../resource/old/Courier_10_14/116_10_14.bmp \
    ../../../resource/old/Courier_10_14/117_10_14.bmp \
    ../../../resource/old/Courier_10_14/118_10_14.bmp \
    ../../../resource/old/Courier_10_14/119_10_14.bmp \
    ../../../resource/old/Courier_10_14/120_10_14.bmp \
    ../../../resource/old/Courier_10_14/121_10_14.bmp \
    ../../../resource/old/Courier_10_14/122_10_14.bmp \
    ../../../resource/old/Courier_10_14/123_10_14.bmp \
    ../../../resource/old/Courier_10_14/124_10_14.bmp \
    ../../../resource/old/Courier_10_14/125_10_14.bmp \
    ../../../resource/old/Courier_10_14/126_10_14.bmp \
    ../../../resource/old/Courier_10_14/32_10_14.bmp \
    ../../../resource/old/Courier_10_14/33_10_14.bmp \
    ../../../resource/old/Courier_10_14/34_10_14.bmp \
    ../../../resource/old/Courier_10_14/35_10_14.bmp \
    ../../../resource/old/Courier_10_14/36_10_14.bmp \
    ../../../resource/old/Courier_10_14/37_10_14.bmp \
    ../../../resource/old/Courier_10_14/38_10_14.bmp \
    ../../../resource/old/Courier_10_14/39_10_14.bmp \
    ../../../resource/old/Courier_10_14/40_10_14.bmp \
    ../../../resource/old/Courier_10_14/41_10_14.bmp \
    ../../../resource/old/Courier_10_14/42_10_14.bmp \
    ../../../resource/old/Courier_10_14/43_10_14.bmp \
    ../../../resource/old/Courier_10_14/44_10_14.bmp \
    ../../../resource/old/Courier_10_14/45_10_14.bmp \
    ../../../resource/old/Courier_10_14/46_10_14.bmp \
    ../../../resource/old/Courier_10_14/47_10_14.bmp \
    ../../../resource/old/Courier_10_14/48_10_14.bmp \
    ../../../resource/old/Courier_10_14/49_10_14.bmp \
    ../../../resource/old/Courier_10_14/50_10_14.bmp \
    ../../../resource/old/Courier_10_14/51_10_14.bmp \
    ../../../resource/old/Courier_10_14/52_10_14.bmp \
    ../../../resource/old/Courier_10_14/53_10_14.bmp \
    ../../../resource/old/Courier_10_14/54_10_14.bmp \
    ../../../resource/old/Courier_10_14/55_10_14.bmp \
    ../../../resource/old/Courier_10_14/56_10_14.bmp \
    ../../../resource/old/Courier_10_14/57_10_14.bmp \
    ../../../resource/old/Courier_10_14/58_10_14.bmp \
    ../../../resource/old/Courier_10_14/59_10_14.bmp \
    ../../../resource/old/Courier_10_14/60_10_14.bmp \
    ../../../resource/old/Courier_10_14/61_10_14.bmp \
    ../../../resource/old/Courier_10_14/62_10_14.bmp \
    ../../../resource/old/Courier_10_14/63_10_14.bmp \
    ../../../resource/old/Courier_10_14/64_10_14.bmp \
    ../../../resource/old/Courier_10_14/65_10_14.bmp \
    ../../../resource/old/Courier_10_14/66_10_14.bmp \
    ../../../resource/old/Courier_10_14/67_10_14.bmp \
    ../../../resource/old/Courier_10_14/68_10_14.bmp \
    ../../../resource/old/Courier_10_14/69_10_14.bmp \
    ../../../resource/old/Courier_10_14/70_10_14.bmp \
    ../../../resource/old/Courier_10_14/71_10_14.bmp \
    ../../../resource/old/Courier_10_14/72_10_14.bmp \
    ../../../resource/old/Courier_10_14/73_10_14.bmp \
    ../../../resource/old/Courier_10_14/74_10_14.bmp \
    ../../../resource/old/Courier_10_14/75_10_14.bmp \
    ../../../resource/old/Courier_10_14/76_10_14.bmp \
    ../../../resource/old/Courier_10_14/77_10_14.bmp \
    ../../../resource/old/Courier_10_14/78_10_14.bmp \
    ../../../resource/old/Courier_10_14/79_10_14.bmp \
    ../../../resource/old/Courier_10_14/80_10_14.bmp \
    ../../../resource/old/Courier_10_14/81_10_14.bmp \
    ../../../resource/old/Courier_10_14/82_10_14.bmp \
    ../../../resource/old/Courier_10_14/83_10_14.bmp \
    ../../../resource/old/Courier_10_14/84_10_14.bmp \
    ../../../resource/old/Courier_10_14/85_10_14.bmp \
    ../../../resource/old/Courier_10_14/86_10_14.bmp \
    ../../../resource/old/Courier_10_14/87_10_14.bmp \
    ../../../resource/old/Courier_10_14/88_10_14.bmp \
    ../../../resource/old/Courier_10_14/89_10_14.bmp \
    ../../../resource/old/Courier_10_14/90_10_14.bmp \
    ../../../resource/old/Courier_10_14/91_10_14.bmp \
    ../../../resource/old/Courier_10_14/92_10_14.bmp \
    ../../../resource/old/Courier_10_14/93_10_14.bmp \
    ../../../resource/old/Courier_10_14/94_10_14.bmp \
    ../../../resource/old/Courier_10_14/95_10_14.bmp \
    ../../../resource/old/Courier_10_14/96_10_14.bmp \
    ../../../resource/old/Courier_10_14/97_10_14.bmp \
    ../../../resource/old/Courier_10_14/98_10_14.bmp \
    ../../../resource/old/Courier_10_14/99_10_14.bmp \
    ../../../resource/old/AND.BMP \
    ../../../resource/old/BORDER.BMP \
    ../../../resource/old/CLOSE.BMP \
    ../../../resource/old/COIL.BMP \
    ../../../resource/old/CONNECT.BMP \
    ../../../resource/old/CONTACTNC.BMP \
    ../../../resource/old/CONTACTNO.BMP \
    ../../../resource/old/COPY.BMP \
    ../../../resource/old/CROSS.BMP \
    ../../../resource/old/CUT.BMP \
    ../../../resource/old/DELETE.BMP \
    ../../../resource/old/EXIT.BMP \
    ../../../resource/old/HORZ.BMP \
    ../../../resource/old/ICONIZE.BMP \
    ../../../resource/old/L1.BMP \
    ../../../resource/old/L2.BMP \
    ../../../resource/old/L3.BMP \
    ../../../resource/old/L4.BMP \
    ../../../resource/old/MAXIMIZE.BMP \
    ../../../resource/old/MEDIALIZE.BMP \
    ../../../resource/old/MOVE.BMP \
    ../../../resource/old/NEW.BMP \
    ../../../resource/old/OPEN.BMP \
    ../../../resource/old/OR.BMP \
    ../../../resource/old/PASTE.BMP \
    ../../../resource/old/REDO.BMP \
    ../../../resource/old/RELE.BMP \
    ../../../resource/old/SAVE.BMP \
    ../../../resource/old/SELCONNECT.BMP \
    ../../../resource/old/SETTING.BMP \
    ../../../resource/old/SHL.BMP \
    ../../../resource/old/SHR.BMP \
    ../../../resource/old/SVD.BMP \
    ../../../resource/old/SVU.BMP \
    ../../../resource/old/T1.BMP \
    ../../../resource/old/T2.BMP \
    ../../../resource/old/T3.BMP \
    ../../../resource/old/T4.BMP \
    ../../../resource/old/UNDO.BMP \
    ../../../resource/old/VERT.BMP \
    ../../../linkresource.sh \
    ../../../n2resource.sh
