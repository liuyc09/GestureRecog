#-------------------------------------------------
#
# Project created by QtCreator 2013-02-11T21:16:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GesturePasswords
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    include/skindetectcontroller.cpp \
    include/skindetector.cpp \
    src/maskselection.cpp \
    src/gesturedetector.cpp \

HEADERS  += \
    include/colorhistogram.h \
    include/skindetectcontroller.h \
    include/skindetector.h \
    src/gesturedetector.h \
    src/maskselection.h \
    include/hand.h \
    include/passwordcheck.h \
    src/passwordcheck.h

FORMS    += \
    src/gesturedetector.ui \
    src/maskselection.ui



INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_objdetect \
   -lopencv_calib3d
