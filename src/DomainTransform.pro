TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle


# uncomment to activate per function profiling & output
########################################################
    DEFINES += DO_FUNCTION_PROFILING


# release settings
########################################################
    release {
        # remove asserts from release code
        DEFINES += NDEBUG
    }
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
    QMAKE_CXXFLAGS_RELEASE += -m64

# debug settings
########################################################
    debug {
    }
    QMAKE_CXXFLAGS_DEBUG -= -O2
    QMAKE_CXXFLAGS_DEBUG += -O3
    QMAKE_CXXFLAGS_DEBUG += -m64


# we really don't want no auto vectorisation for now :P
########################################################
    QMAKE_CXXFLAGS_RELEASE += -fno-tree-vectorize
    QMAKE_CXXFLAGS_DEBUG += -fno-tree-vectorize
    QMAKE_CXXFLAGS += -fno-tree-vectorize


DESTDIR = ../build
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc

INCLUDEPATH += external/

LIBS+= -lpng

SOURCES += main.cpp \
    external/io_png/io_png.c

HEADERS += \
    external/tclap/CmdLine.h \
    external/io_png/io_png.h \
    Image.h \
    NC.h \
    Exception.h \
    Mat2.h \
    RF.h \
    common.h\
    rdtsc.h \
    FunctionProfiling.h

FORMS +=

