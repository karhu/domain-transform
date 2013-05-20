cmake_minimum_required(VERSION 2.8)

######### Project Setup
project(DomainTransform)

IF(NOT CMAKE_BUILD_TYPE)
#  SET(CMAKE_BUILD_TYPE "DEBUG")
  SET(CMAKE_BUILD_TYPE "RELEASE")
  #SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
  #SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -O3 -march=corei7-avx")

# enable ALL warnings
ADD_DEFINITIONS("-Wall")

#ADD_DEFINITIONS("-std=c++0x") # alternative please test this...

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

#SET(ADDITINAL_LINK_FLAGS)
#if(APPLE)
#    SET (CMAKE_FIND_LIBRARY_SUFFIXES .dylib)
#    set (CMAKE_OSX_DEPLOYMENT_TARGET=10.8)
##    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#    LINK_DIRECTORIES(/usr/lib /usr/local/lib)
#
#    SET(ADDITINAL_LINK_FLAGS "-lpython2.7")
#else(APPLE)
#    SET (CMAKE_FIND_LIBRARY_SUFFIXES .so)
#    # -lrt is for the CLOCK
#    # -lpython is for the python bindings
#    SET(ADDITINAL_LINK_FLAGS "-lrt" "-lpython2.7")
#endif(APPLE)

# gprof
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#set(CMAKE_EXE_FLAGS "${CMAKE_EXE_FLAGS} -pg")

######### Code & include_dirs
include_directories(
        src
        src/external
        ${PNG_INCLUDE_DIR}
        )

set(HEADERS
    src/external/tclap/CmdLine.h
    src/external/io_png/io_png.h
    src/Image.h
    src/NC.h
    src/Exception.h
    src/Mat2.h
    src/RF.h
    src/common.h
    src/rdtsc.h
    src/FunctionProfiling.h
    )

set(SOURCES
#    ${COMMON}
    src/main.cpp
    src/external/io_png/io_png.c
    )

######### Main Executable
add_executable(DomainTransform ${SOURCES})

target_link_libraries(DomainTransform
        ${PNG_LIBRARY}
        )

#TARGET_LINK_LIBRARIES(hpc_queues 
#        ${Boost_LIBRARIES}
#        ${ADDITINAL_LINK_FLAGS}
#        pthread
#        tbb
#        json_spirit
#)

