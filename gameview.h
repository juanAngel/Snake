#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "view.h"
#include "snake.h"

typedef struct gameView:view{
    drawableSnake snake;
    SDL_Point Applet;
}GameView;

#endif // GAMEVIEW_H
