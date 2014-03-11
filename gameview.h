#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "view.h"
#include "snake.h"

typedef struct GameView:view{
    drawableSnake snake;
    SDL_Point Applet;
    bool start;
}GameView;

void initGameView(GameView* game, SDL_Rect *r, event::EventLoop *defaultLoop);


#endif // GAMEVIEW_H
