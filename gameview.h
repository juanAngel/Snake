#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "view.h"
#include "snake.h"

typedef struct GameView:view{
    drawableSnake snake;
    SDL_Point Applet;
}GameView;

void initGameView(GameView* game, SDL_Rect *r);


#endif // GAMEVIEW_H
