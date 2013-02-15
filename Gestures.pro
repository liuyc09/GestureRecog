#-------------------------------------------------
#
# Project created by QtCreator 2013-02-11T21:16:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gestures
TEMPLATE = app


SOURCES += main.cpp\
    colordetector.cpp \
    colordetectcontroller.cpp \
    skindetectcontroller.cpp \
    skindetector.cpp \
    gesturedetection.cpp \
    MaskSelection.cpp

HEADERS  += \
    colordetector.h \
    colordetectcontroller.h \
    colorhistogram.h \
    skindetectcontroller.h \
    skindetector.h \
    gesturedetection.h \
    maskselection.h

FORMS    += \
    gesturedetection.ui \
    maskselection.ui



INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_calib3d
