TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG += -O3
QMAKE_CXXFLAGS_DEBUG += -fno-tree-vectorize
QMAKE_CXXFLAGS_DEBUG += -m64


#QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.7.sdk

# uncomment to activate per function profiling & output
DEFINES += DO_FUNCTION_PROFILING

release {
    # remove asserts from release code
    DEFINES += NDEBUG
}

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
    Exception.h \
    Mat2.h \
    RF.h \
    common.h\
    rdtsc.h \
    FunctionProfiling.h

FORMS += \
    mainwindow.ui

