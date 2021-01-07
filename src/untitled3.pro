TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS+= `pkg-config --libs opencv`
SOURCES += \
        functions.cpp \
        main.cpp

HEADERS += \
    shapeContour.h
