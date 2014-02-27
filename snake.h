#ifndef SNAKE_H
#define SNAKE_H

#include "snake_global.h"
#include "renderer.h"


typedef struct snakeNode{
    snakeNode* next;
    SDL_Point pos;
}snakeNode;
typedef struct snakeHead:snakeNode{
    SDL_Point vecDirector;
    double speed;
    SDL_TimerID timer;
}snakeHead;

typedef struct drawableSnake: renderer::drawable{
    snakeHead* snake;
}drawableSnake;

void initDrawableSnake(drawableSnake* d, snakeHead* head);

//Function for draw Snake and Applet
void drawSnake(SDL_Renderer* render,snakeHead* snake);
void drawApplet(SDL_Renderer* render,SDL_Point* AppletPos);

//Manejo de la serpiente
void moveSnake(snakeHead* snake, SDL_Rect *Display, SDL_Point *Applet);
void startSnake(snakeHead* snake, SDL_Rect *display, SDL_Point *applet);
void addNode(snakeHead* snake);
//Manejo de la manzana
void launchApplet(SDL_Point* Applet,SDL_Rect* display);

//Constantes que sirven para setear el vector director en una direccion concreta
extern const SDL_Point direcUp,direcDown
                        ,direcRight,direcLetf;

#endif // SNAKE_H
