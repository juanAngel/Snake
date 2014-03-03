#ifndef ESCENE_H
#define ESCENE_H

#include "snake_global.h"
#include "snake.h"

typedef struct scene{
    SDL_Color backgroundColor;
    rectangle r;
    snakeHead sake;
    SDL_Point Applet;

    //Matriz que indica si en determinada posicion se puede pasar o no
    //gameField field;
}scene;

void initScene(SDL_Rect* display,scene* escena);


#endif // ESCENE_H
