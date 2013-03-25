TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt


#QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.7.sdk

INCLUDEPATH += external/

LIBS+= -lpng

SOURCES += main.cpp \
    external/io_png/io_png.c \
    mainwindow.cpp

HEADERS += \
    external/tclap/CmdLine.h \
    external/io_png/io_png.h \
    Image.h \
    mainwindow.h \
    NC.h \
    Exception.h

FORMS += \
    mainwindow.ui

