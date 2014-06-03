# this project is used to wrap cosm psf code
project(CosmPsfWrapperProject)

# Find Swig
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

# Find JNI
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# Find Java
find_package(Java REQUIRED)
INCLUDE(UseJava)

# Find Cosm
# FIND_PATH( COSM_PATH $ENV{COSM_DIR} [DOC "Cosm directory"] )
set (COSM_PATH "../COSM-0.7.4-Source")

set(COSM_INCLUDE_DIRS
  "${COSM_PATH}/psf"
  "${COSM_PATH}/psf/psf"
  "${COSM_PATH}/util"
)

# Find Blitz
FIND_PATH( BLITZ_DIR $ENV{BLITZ_DIR} [DOC "Blitz directory path"])

include_directories(${COSM_INCLUDE_DIRS} ${BLITZ_DIR})

add_definitions(-fPIC)

set( QUADPACK_PATH
  ${COSM_PATH}/util/quadpack
  )
file(GLOB QUADPACK_C_SRC
  ${QUADPACK_PATH}/*.c
)
file(GLOB QUADPACK_CXX_SRC
  ${QUADPACK_PATH}/*.cxx
)
set( QUADPACK_SRC
  ${QUADPACK_C_SRC}
  ${QUADPACK_CXX_SRC} 
)

set( PSF_SRC ${COSM_PATH}/psf/psf)
set( XCOSM_SRC ${COSM_PATH}/psf/xcosm)

SET(PSF_LIB_SRCS ${PSF_SRC}/dqagFunction 
	${XCOSM_SRC}/rdlinesum
	${XCOSM_SRC}/rdcircsum
    ${XCOSM_SRC}/rotnone
    ${XCOSM_SRC}/rotsum
    ${XCOSM_SRC}/rotdiskline
    # ${XCOSM_SRC}/rotdicline
   # ${XCOSM_SRC}/rotinfdicline
    ${XCOSM_SRC}/rotdiskcirc
    ${XCOSM_SRC}/spline
    ${XCOSM_SRC}/real_fft3d
    ${XCOSM_SRC}/write_dataset
    ${XCOSM_SRC}/misc
    ${XCOSM_SRC}/tmp
    # ${XCOSM_SRC}/fftn
    )

# add the cosm wrapper library
add_library(CosmPsfWrapper SHARED ${PSF_LIB_SRCS} ${QUADPACK_SRC} CosmPsfWrapper)

# tell swig to put the .java files into javasrc directory
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/javasrc)

# tell swig to use com.deconware.wrappers namespace
SET(CMAKE_SWIG_FLAGS -package com.deconware.wrappers)

# add the swig java wrapper module
swig_add_module(CosmPsfJavaSwig java CosmPsfWrapper.i "CosmPsfWrapper.cpp")

# glob all the java files
FILE(GLOB JAVA_SOURCE ${CMAKE_SWIG_OUTDIR}/*.java)
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.6" "-target" "1.6")
# make the jar wrapper
add_jar(CosmPsfJavaWrapper ${JAVA_SOURCE} OUTPUT_DIR ${JAR_PATH})

add_dependencies(CosmPsfJavaWrapper CosmPsfJavaSwig)

