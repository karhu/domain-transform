TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += external/

LIBS+= -lpng

SOURCES += main.cpp \
    external/io_png/io_png.c

HEADERS += \
    external/tclap/CmdLine.h \
    external/io_png/io_png.h \
    Image.h

