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
        gesturerecognition.cpp \
    colordetector.cpp \
    colordetectcontroller.cpp

HEADERS  += gesturerecognition.h \
    colordetector.h \
    colordetectcontroller.h

FORMS    += gesturerecognition.ui



INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_calib3d
