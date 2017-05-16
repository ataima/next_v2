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
../../source/n2capturepos.cpp            \



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
../../header/n2capturepos.h            \


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



N2XML =     \
../../../dist/conf_utf8.xml     \
../../../dist/conf_utf16.xml

N2BMP =    \
../../../dist/old/AND.BMP           \
../../../dist/old/CROSS.BMP         \
../../../dist/old/L4.BMP            \
../../../dist/old/RELE.BMP          \
../../../dist/old/T2.BMP            \
../../../dist/old/BORDER.BMP        \
../../../dist/old/CUT.BMP           \
../../../dist/old/MAXIMIZE.BMP      \
../../../dist/old/SAVE.BMP          \
../../../dist/old/T3.BMP            \
../../../dist/old/CLOSE.BMP         \
../../../dist/old/DELETE.BMP        \
../../../dist/old/MEDIALIZE.BMP     \
../../../dist/old/SELCONNECT.BMP    \
../../../dist/old/T4.BMP            \
../../../dist/old/COIL.BMP          \
../../../dist/old/EXIT.BMP          \
../../../dist/old/MOVE.BMP          \
../../../dist/old/SETTING.BMP       \
../../../dist/old/UNDO.BMP          \
../../../dist/old/CONNECT.BMP       \
../../../dist/old/HORZ.BMP          \
../../../dist/old/NEW.BMP           \
../../../dist/old/SHL.BMP           \
../../../dist/old/VERT.BMP          \
../../../dist/old/CONTACTNC.BMP     \
../../../dist/old/ICONIZE.BMP       \
../../../dist/old/OPEN.BMP          \
../../../dist/old/SHR.BMP           \
../../../dist/old/REDO.BMP          \
../../../dist/old/CONTACTNO.BMP     \
../../../dist/old/L1.BMP            \
 ../../../dist/old/OR.BMP           \
../../../dist/old/SVD.BMP           \
../../../dist/old/T1.BMP            \
../../../dist/old/COPY.BMP          \
../../../dist/old/L2.BMP            \
../../../dist/old/PASTE.BMP         \
../../../dist/old/SVU.BMP           \
../../../dist/old/L3.BMP


N2FONTS =    \
../../../dist/old/Courier_10_14/100_10_14.bmp       \
../../../dist/old/Courier_10_14/119_10_14.bmp       \
../../../dist/old/Courier_10_14/43_10_14.bmp        \
../../../dist/old/Courier_10_14/62_10_14.bmp        \
../../../dist/old/Courier_10_14/81_10_14.bmp        \
../../../dist/old/Courier_10_14/101_10_14.bmp       \
../../../dist/old/Courier_10_14/120_10_14.bmp       \
../../../dist/old/Courier_10_14/44_10_14.bmp        \
../../../dist/old/Courier_10_14/63_10_14.bmp        \
../../../dist/old/Courier_10_14/82_10_14.bmp        \
../../../dist/old/Courier_10_14/102_10_14.bmp       \
../../../dist/old/Courier_10_14/121_10_14.bmp       \
../../../dist/old/Courier_10_14/45_10_14.bmp        \
../../../dist/old/Courier_10_14/64_10_14.bmp        \
../../../dist/old/Courier_10_14/83_10_14.bmp        \
../../../dist/old/Courier_10_14/103_10_14.bmp       \
../../../dist/old/Courier_10_14/122_10_14.bmp       \
../../../dist/old/Courier_10_14/46_10_14.bmp        \
../../../dist/old/Courier_10_14/65_10_14.bmp        \
../../../dist/old/Courier_10_14/84_10_14.bmp        \
../../../dist/old/Courier_10_14/104_10_14.bmp       \
../../../dist/old/Courier_10_14/123_10_14.bmp       \
../../../dist/old/Courier_10_14/47_10_14.bmp        \
../../../dist/old/Courier_10_14/66_10_14.bmp        \
../../../dist/old/Courier_10_14/85_10_14.bmp        \
../../../dist/old/Courier_10_14/105_10_14.bmp       \
../../../dist/old/Courier_10_14/124_10_14.bmp       \
../../../dist/old/Courier_10_14/48_10_14.bmp        \
../../../dist/old/Courier_10_14/67_10_14.bmp        \
../../../dist/old/Courier_10_14/86_10_14.bmp        \
../../../dist/old/Courier_10_14/106_10_14.bmp       \
../../../dist/old/Courier_10_14/125_10_14.bmp       \
../../../dist/old/Courier_10_14/49_10_14.bmp        \
../../../dist/old/Courier_10_14/68_10_14.bmp        \
../../../dist/old/Courier_10_14/87_10_14.bmp        \
../../../dist/old/Courier_10_14/107_10_14.bmp       \
../../../dist/old/Courier_10_14/126_10_14.bmp       \
../../../dist/old/Courier_10_14/50_10_14.bmp        \
../../../dist/old/Courier_10_14/69_10_14.bmp        \
../../../dist/old/Courier_10_14/88_10_14.bmp        \
../../../dist/old/Courier_10_14/108_10_14.bmp       \
../../../dist/old/Courier_10_14/32_10_14.bmp        \
../../../dist/old/Courier_10_14/51_10_14.bmp        \
../../../dist/old/Courier_10_14/70_10_14.bmp        \
../../../dist/old/Courier_10_14/89_10_14.bmp        \
../../../dist/old/Courier_10_14/109_10_14.bmp       \
../../../dist/old/Courier_10_14/33_10_14.bmp        \
../../../dist/old/Courier_10_14/52_10_14.bmp        \
../../../dist/old/Courier_10_14/71_10_14.bmp        \
../../../dist/old/Courier_10_14/90_10_14.bmp        \
../../../dist/old/Courier_10_14/110_10_14.bmp       \
../../../dist/old/Courier_10_14/34_10_14.bmp        \
../../../dist/old/Courier_10_14/53_10_14.bmp        \
../../../dist/old/Courier_10_14/72_10_14.bmp        \
../../../dist/old/Courier_10_14/91_10_14.bmp        \
../../../dist/old/Courier_10_14/111_10_14.bmp       \
../../../dist/old/Courier_10_14/35_10_14.bmp        \
../../../dist/old/Courier_10_14/54_10_14.bmp        \
../../../dist/old/Courier_10_14/73_10_14.bmp        \
../../../dist/old/Courier_10_14/92_10_14.bmp        \
../../../dist/old/Courier_10_14/112_10_14.bmp       \
../../../dist/old/Courier_10_14/36_10_14.bmp        \
../../../dist/old/Courier_10_14/55_10_14.bmp        \
../../../dist/old/Courier_10_14/74_10_14.bmp        \
../../../dist/old/Courier_10_14/93_10_14.bmp        \
../../../dist/old/Courier_10_14/113_10_14.bmp       \
../../../dist/old/Courier_10_14/37_10_14.bmp        \
../../../dist/old/Courier_10_14/56_10_14.bmp        \
../../../dist/old/Courier_10_14/75_10_14.bmp        \
../../../dist/old/Courier_10_14/94_10_14.bmp        \
../../../dist/old/Courier_10_14/114_10_14.bmp       \
../../../dist/old/Courier_10_14/38_10_14.bmp        \
../../../dist/old/Courier_10_14/57_10_14.bmp        \
../../../dist/old/Courier_10_14/76_10_14.bmp        \
../../../dist/old/Courier_10_14/95_10_14.bmp        \
../../../dist/old/Courier_10_14/115_10_14.bmp       \
../../../dist/old/Courier_10_14/39_10_14.bmp        \
../../../dist/old/Courier_10_14/58_10_14.bmp        \
../../../dist/old/Courier_10_14/77_10_14.bmp        \
../../../dist/old/Courier_10_14/96_10_14.bmp        \
../../../dist/old/Courier_10_14/116_10_14.bmp       \
../../../dist/old/Courier_10_14/40_10_14.bmp        \
../../../dist/old/Courier_10_14/59_10_14.bmp        \
../../../dist/old/Courier_10_14/78_10_14.bmp        \
../../../dist/old/Courier_10_14/97_10_14.bmp        \
../../../dist/old/Courier_10_14/117_10_14.bmp       \
../../../dist/old/Courier_10_14/41_10_14.bmp        \
../../../dist/old/Courier_10_14/60_10_14.bmp        \
../../../dist/old/Courier_10_14/79_10_14.bmp        \
../../../dist/old/Courier_10_14/98_10_14.bmp        \
../../../dist/old/Courier_10_14/118_10_14.bmp       \
../../../dist/old/Courier_10_14/42_10_14.bmp        \
../../../dist/old/Courier_10_14/61_10_14.bmp        \
../../../dist/old/Courier_10_14/80_10_14.bmp        \
../../../dist/old/Courier_10_14/99_10_14.bmp




android {
addon_1.files = $$N2XML
addon_1.path = $$[PWD]/assets
INSTALLS += addon_1
}


android {
addon_2.files = $$N2BMP
addon_2.path = $$[PWD]/assets/old
INSTALLS += addon_2
}

android {
addon_3.files = $$N2FONTS
addon_3.path = $$[PWD]/assets/old/Courier_10_14
INSTALLS += addon_3
}


INCLUDEPATH -= $$QMAKE_DEFAULT_INCDIRS
