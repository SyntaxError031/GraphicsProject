#-------------------------------------------------
#
# Project created by QtCreator 2018-10-21T19:38:14
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drawGraph
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mywidget.cpp \
    line.cpp \
    circle.cpp \
    ellipse.cpp \
    rectangle.cpp \
    polygon.cpp \
    curve.cpp \
    threedwidget.cpp \
    threedwindow.cpp

HEADERS += \
        mainwindow.h \
    mywidget.h \
    simplefigure.h \
    line.h \
    circle.h \
    ellipse.h \
    rectangle.h \
    polygon.h \
    curve.h \
    threedwidget.h \
    threedwindow.h

FORMS += \
        mainwindow.ui \
    mywidget.ui \
    threedwidget.ui \
    threedwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myimages.qrc


LIBS += -lglut32 \
    -lopengl32 \
    -lglu32

