#-------------------------------------------------
#
# Project created by QtCreator 2015-11-21T14:39:27
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImgDatasetGen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagetransformation.cpp \
    filemanipulation.cpp \
    log.cpp \
    CThinPlateSpline.cpp

HEADERS  += mainwindow.h \
    imagetransformation.h \
    filemanipulation.h \
    log.h \
    constants.h \
    CThinPlateSpline.h

FORMS    += mainwindow.ui


INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

LIBS += -L"/usr/local/lib/"
LIBS += -lopencv_calib3d
LIBS += -lopencv_contrib
LIBS += -lopencv_core
LIBS += -lopencv_features2d
LIBS += -lopencv_flann
LIBS += -lopencv_gpu
LIBS += -lopencv_highgui
LIBS += -lopencv_imgproc
LIBS += -lopencv_legacy
LIBS += -lopencv_ml
LIBS += -lopencv_nonfree
LIBS += -lopencv_objdetect
LIBS += -lopencv_ocl
LIBS += -lopencv_photo
LIBS += -lopencv_stitching
LIBS += -lopencv_superres
LIBS += -lopencv_ts
LIBS += -lopencv_video
LIBS += -lopencv_videostab


LIBS += -L"/usr/local/cuda/lib64/"
LIBS += -lcufft
LIBS += -lnpps
LIBS += -lnppi
LIBS += -lnppc
LIBS += -lcudart
LIBS += -lrt
LIBS += -lpthread
LIBS += -lm
LIBS += -ldl

INCLUDEPATH += /usr/local/include/log4cpp
LIBS += -L"/usr/local/lib"
LIBS += -llog4cpp

DISTFILES += \
    log4cpp.properties

