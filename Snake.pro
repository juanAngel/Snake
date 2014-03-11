TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11


SOURCES += main.cpp \
    renderer.cpp \
    snake.cpp \
    scene.cpp \
    view.cpp \
    eventloop.cpp \
    gameview.cpp \
    guiview.cpp \
    SDL_ttf.c

#unix:LIBS += -lSDL2

HEADERS += \
    renderer.h \
    snake_global.h \
    scene.h \
    snake.h \
    eventloop.h \
    view.h \
    gameview.h \
    guiview.h \
    SDL_ttf.h

LIBS += -lSDL2 -lfreetype
INCLUDEPATH += /usr/include/freetype2/

DEFINES += USE_FREETYPE_ERRORS

#LIBS = -landroid


