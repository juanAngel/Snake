#include "snake.h"
#include <stdlib.h>

typedef struct snakeContext{
    Snake* snake;
    SDL_Rect* display;
    SDL_Point* applet;
}snakeContext;


const SDL_Point direcUp = {0,-1}
            ,direcDown  = {0,1}
            ,direcRight = {1,0}
            ,direcLetf  = {-1,0};

int runSnake(int /*interval*/,snakeContext* context){
    moveSnake(context->snake,context->display,context->applet);

    renderer::sendUpdateWindows();

    return context->snake->delay;
}

void startSnake(Snake* snake,SDL_Rect* display,SDL_Point* applet){
    snakeContext* context = (snakeContext*)malloc(sizeof(*context));

    context->snake = snake;
    context->display = display;
    context->applet = applet;
    snake->delay = 100;

    snake->timer = SDL_AddTimer(snake->delay,(SDL_TimerCallback)&runSnake,context);

    Mix_PlayMusic(snake->music,-1);

    //Agrego nodos a la seriente hasta conseguir los buscados
    for (int i = 0; i < snakeInitialLength-1; ++i)
        addNodeHead(snake);
}
void removeAllSnakeNodes(Snake* snake){
    if(snake){
        for(snakeNode* current = snake->head,*old;current;){
            old = current;
            current = current->next;
            free(old);
        }
        snake->head = snake->tail = nullptr;
    }
}

void drawApplet(renderer::DrawContext context,SDL_Point* AppletPos){
    if(context && AppletPos){
        SDL_Rect r = {AppletPos->x,AppletPos->y,snakeSize,snakeSize};
        SDL_Color color = {0xff,0,0,0xff};
        renderer::fillRect(context,&r,&color);
    }
}
void renderSnake(renderer::DrawContext context,drawableSnake* drawableObject){
    if(context && drawableObject){
        snakeNode* node = drawableObject->snake.head;
        SDL_Rect r;
        r.w = r.h = snakeSize-1;
        SDL_Color color = {0,0xff,0,0xff};
        do{
            r.x = node->pos.x;
            r.y = node->pos.y;
            //Dibujo el nodo de la serpiente
            renderer::fillRect(context,&r,&color);

            node = node->next;
        }while(node);
        r.x = drawableObject->snake.tail->pos.x+2;
        r.y = drawableObject->snake.tail->pos.y+2;
        r.w = r.h = snakeSize-4;
        color.r = color.b = color.g;
        renderer::fillRect(context,&r,&color);
    }
}
void moveSnake(Snake* snake,SDL_Rect* Display,SDL_Point* Applet){
    if(snake && Display && Applet){
        SDL_Point nextPos;
        snakeNode* node = snake->tail;

        if(!(snake->vecDirector.x || snake->vecDirector.y || snake->speed)){
            printf("Snake sin velocidad o direccion\n");
            return;
        }
        //Calculo la nueva posicion de la cabeza de la serpiente
        nextPos.x = node->pos.x+snake->vecDirector.x*snake->speed;
        nextPos.y = node->pos.y+snake->vecDirector.y*snake->speed;

        //Testeo la colision con las paredes del escenario
        if(nextPos.x < 0 || nextPos.y < 0
                || nextPos.x > Display->w || nextPos.y > Display->h
                || snake->field.buff[(nextPos.x/snakeSize)+(nextPos.y/snakeSize)*snake->field.s.w]){
            SDL_Event e;
            e.type = gameOverEventID;
            SDL_PushEvent(&e);
        }else{
            moveHeadToTail(snake);
            SDL_Point* pos = &snake->tail->pos;
            //Muevo la posicion de el nodo que va a ser la cabea
            snake->field.buff[(pos->x/snakeSize)+(pos->y/snakeSize)*snake->field.s.w]--;
            *pos = nextPos;
            snake->field.buff[(pos->x/snakeSize)+(pos->y/snakeSize)*snake->field.s.w]++;

            //Testeo la colision con la manzana
            if(Applet->x == nextPos.x && nextPos.y == Applet->y){
                //Si me como una manzana, crezco y tiro otra
                for(int i = 0;i<4;i++)
                    addNodeHead(snake);
                launchApplet(Applet,Display);
                if(!(snake->nAppletMeals%5) && snake->delay >10)
                    snake->delay -= snake->delay/10;
                //Aumento el contador de manzanas comidas
                snake->nAppletMeals++;
                printf("Manzanas comidas %u\n",snake->nAppletMeals);
            }
        }

    }
}
void launchApplet(SDL_Point* Applet,SDL_Rect* display){
    int r = rand()%(display->w*(display->h));
    Applet->x = r%display->w;
    Applet->y = r/display->w;

    //Alineo la manzana a multiplos del snakeSize
    Applet->x -= Applet->x%snakeSize;
    Applet->y -= Applet->y%snakeSize;
    printf("Applet %d,%d\n",Applet->x,Applet->y);
}
void launchSnake(drawableSnake* drawable,SDL_Rect* display){
    if(drawable){
        MapField* field = &drawable->snake.field;
        addNodeHead(&drawable->snake);
        field->s.w = display->w/snakeSize+1;
        field->s.h = display->h/snakeSize+1;
        if(!field->buff)
            field->buff = (byte*)malloc(sizeof(byte)*field->s.w*field->s.h);
        memset(field->buff,false,sizeof(bool)*field->s.w*field->s.h);

        SDL_Point* pos = &drawable->snake.head->pos;
        //Pongo centrado a la serpiente
        pos->x = display->w/2-snakeSize;
        pos->y = display->h/2-snakeSize;
        //Alineo la serpiente a multiplos del snakeSize
        pos->x -= pos->x%snakeSize;
        pos->y -= pos->y%snakeSize;

        //Pongo la posicion de la serpiente
        field->buff[(pos->x/snakeSize)+(pos->y/snakeSize)*field->s.w]++;

    }
}
void resetSnake(drawableSnake* drawable,SDL_Rect* display){
    if(drawable){
        SDL_RemoveTimer(drawable->snake.timer);
        removeAllSnakeNodes(&drawable->snake);
        drawable->snake.nAppletMeals = 0;
        memset(drawable->snake.field.buff,0,drawable->snake.field.s.w*drawable->snake.field.s.h);
        launchSnake(drawable,display);
        Mix_HaltMusic();
    }
}


void initDrawableSnake(drawableSnake *d, Snake *snake){
    if(d){
        d->snake.head = d->snake.tail = nullptr;
        if(snake)
            d->snake = *snake;
        d->renderDrawable = (renderer::DrawFunc)&renderSnake;
    }
}


int getNAppletMeals(drawableSnake *d){
    if(d)
        return d->snake.nAppletMeals;
    return 0;
}


void addNodeHead(Snake *snake){
    snakeNode* node = (snakeNode*)malloc(sizeof(snakeNode));
    if(snake->head){
        *node = *snake->head;
        //umento el indicador de uso del campo
        snake->field.buff[((node->pos.x/snakeSize)+(node->pos.y/snakeSize)*snake->field.s.w)]++;
        node->next = snake->head;
        snake->head = node;
    }else{
        snake->head = snake->tail = node;
        node->pos.x = node->pos.y = 0;
        node->next = nullptr;
    }
}

void addNodeTail(Snake *snake){
    snakeNode* node = (snakeNode*)malloc(sizeof(snakeNode));
    if(snake->head){
        *node = *snake->tail;
        snake->field.buff[(node->pos.x/snakeSize)+(node->pos.y/snakeSize)*snake->field.s.w]++;
        snake->tail->next = node;
        snake->tail = node;
    }else{
        snake->head = snake->tail = node;
        node->next = nullptr;
    }
}
void moveHeadToTail(Snake *snake){
    snake->tail->next = snake->head;
    snake->head = snake->head->next;
    snake->tail = snake->tail->next;
    snake->tail->next = nullptr;
}
