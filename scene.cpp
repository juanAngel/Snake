#include "scene.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

//void initGameField(gameField* field,uint width,uint heigth);

void initScene(SDL_Rect* display,scene* escena){
    if(display && escena){
        snakeHead* snake = &escena->sake;
        //Inicializo los numeros aleatorios
        srand(SDL_GetTicks());
        //Inicializo el campo
        //initGameField(&escena->field,display->w,display->h);

        //test de renderizado de cuadro
        escena->r.x = escena->r.y = 0;
        escena->r.w = escena->r.h = 50;

        escena->r.fillColor.r = escena->r.fillColor.a = 0xff;


        //Pongo centrado a la serpiente
        snake->pos.x = display->w/2-snakeSize;
        snake->pos.y = display->h/2-snakeSize;
        //Alineo la serpiente a multiplos del snakeSize
        snake->pos.x -= snake->pos.x%snakeSize;
        snake->pos.y -= snake->pos.y%snakeSize;

        snake->speed = snakeSize;
        snake->vecDirector = direcRight;

        launchApplet(&escena->Applet,display);

    }
}

//void setAplletPoss(gameField* /*field*/){
//}

//void initGameField(gameField* field
//                   ,uint width,uint heigth){

//    if(field && width && heigth){
//        field->data = (byte*)malloc(width*heigth);
//        field->heigth = heigth;
//        field->width = width;
//        memset(field->data,0,width*heigth);
//    }
//}
