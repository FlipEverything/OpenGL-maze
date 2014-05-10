TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS+=-lGLEW -lGL -lglut -lGLU -lSOIL -lm

SOURCES +=  main.cpp \
    gamedata.cpp \
    element.cpp


HEADERS += \
     main.h \
    gamedata.h \
    element.h

