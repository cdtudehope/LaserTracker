#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T12:30:06
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaserTrackerProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    blobdetect.cpp \
    widget.cpp \
    mypartition.cpp \
    lasertrack.cpp

HEADERS  += blobdetect.h \
    widget.h \
    mypartition.h \
    lasertrack.h

FORMS    += widget.ui

INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv-build\bin\libopencv_core331.dll
LIBS += C:\opencv-build\bin\libopencv_highgui331.dll
LIBS += C:\opencv-build\bin\libopencv_imgcodecs331.dll
LIBS += C:\opencv-build\bin\libopencv_imgproc331.dll
LIBS += C:\opencv-build\bin\libopencv_features2d331.dll
LIBS += C:\opencv-build\bin\libopencv_calib3d331.dll
LIBS += C:\opencv-build\bin\libopencv_video331.dll
LIBS += C:\opencv-build\bin\libopencv_videoio331.dll

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/build
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core331        \
#        -lopencv_highgui331     \
#        -lopencv_imgcodecs331   \
#        -lopencv_imgproc331     \
#        -lopencv_features2d331  \
#        -lopencv_calib3d331

DISTFILES += \
    ../../Pictures/Camera Roll/WIN_20180111_15_53_13_Pro.jpg
