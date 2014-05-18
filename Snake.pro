TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11


SOURCES += main.cpp \
    renderer.cpp \
    snake.cpp \
    view.cpp \
    eventloop.cpp \
    gameview.cpp \
    guiview.cpp \
#    SDL_ttf.c \
    containerTypes.cpp \
    randontree.cpp \
    records.cpp

#unix:LIBS += -lSDL2

HEADERS += \
    renderer.h \
    snake_global.h \
    snake.h \
    eventloop.h \
    view.h \
    gameview.h \
    guiview.h \
    SDL_ttf.h \
    containerTypes.h \
    randontree.h \
    records.h

LIBS += -lSDL2 -lSDL2_mixer -lSDL2_ttf

#LIBS = -landroid

OTHER_FILES += \
    sound/GameOver.ogg \
    sound/mutation.wav \
    sound/snake.ogg \
    font/arial.ttf \
    font/lazy.ttf




