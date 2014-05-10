TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS+=-lGLEW -lGL -lglut -lGLU -lSOIL -lm

SOURCES +=  main.cpp \
    gamedata.cpp \
    gameelement.cpp


HEADERS += \
     main.h \
    gamedata.h \
    gameelement.h

