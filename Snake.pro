TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11


SOURCES += main.cpp \
    renderer.cpp \
    snake.cpp \
    scene.cpp \
    eventloop.c

#unix:LIBS += -lSDL2

HEADERS += \
    renderer.h \
    snake_global.h \
    scene.h \
    snake.h \
    eventloop.h

LIBS += -lSDL2

#LIBS = -landroid


