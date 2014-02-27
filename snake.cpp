#include "snake.h"
#include "scene.h"

typedef struct snakeContext{
    snakeHead* snake;
    SDL_Rect* display;
    SDL_Point* applet;
}snakeContext;


const SDL_Point direcUp = {0,-1}
            ,direcDown  = {0,1}
            ,direcRight = {1,0}
            ,direcLetf  = {-1,0};

int runSnake(int interval,snakeContext* context){
    SDL_UserEvent e;
    e.type = updateEventID;

    moveSnake(context->snake,context->display,context->applet);
    SDL_PushEvent((SDL_Event*)&e);

    return interval;
}

void startSnake(snakeHead* snake,SDL_Rect* display,SDL_Point* applet){
    snakeContext* context = (snakeContext*)malloc(sizeof(*context));

    context->snake = snake;
    context->display = display;
    context->applet = applet;

    snake->timer = SDL_AddTimer(100,(SDL_TimerCallback)&runSnake,context);

    //Agrego nodos a la seriente hasta conseguir los buscados
    for (int i = 0; i < snakeInitialLength-1; ++i)
        addNode(snake);
}
void addNode(snakeHead* snake){
    snakeNode* newNode,*node;
    //Compruebo los parametros
    if(snake){
        //Creo el nuevo nodo de la serpiente
        newNode = (snakeNode*)malloc(sizeof(*newNode));
        memset(newNode,0,sizeof(snakeNode));
        //Busco el ultimo elemento de la cola de la serpiente
        node = snake;
        while (node->next)
            node = node->next;

        newNode->pos = node->pos;
        node->next = newNode;
    }
}
void drawApplet(SDL_Renderer* render,SDL_Point* AppletPos){
    if(render && AppletPos){
        SDL_Rect r = {AppletPos->x,AppletPos->y,snakeSize,snakeSize};

        SDL_SetRenderDrawColor(render,0xff,0,0,0xff);
        SDL_RenderFillRect(render,&r);
    }
}
void renderSnake(renderer::DrawContext* /*context*/,renderer::drawable* /*drawableObject*/){

}

void drawSnake(SDL_Renderer* render,snakeHead* snake){
    if(render && snake){
        snakeNode* node = snake;
        SDL_Rect r;
        r.w = r.h = snakeSize;
        do{
            r.x = node->pos.x;
            r.y = node->pos.y;
            //Dibujo el nodo de la serpiente
            SDL_SetRenderDrawColor(render,0,0xff,0,0xff);
            SDL_RenderFillRect(render,&r);

            node = node->next;
        }while(node);
    }
}
void moveSnake(snakeHead* snake,SDL_Rect* Display,SDL_Point* Applet){
    if(snake && Display && Applet){
        snakeNode* node = snake;
        SDL_Point lastPos,tmp;
        bool colision = false;

        //Muevo la posicion de la cabeza
        lastPos = node->pos;
        node->pos.x += (int)(snake->vecDirector.x*snake->speed);
        node->pos.y += (int)(snake->vecDirector.y*snake->speed);
        //Testeo la colision con la manzana
        if(Applet->x == node->pos.x && node->pos.y == Applet->y){
            addNode(snake);
            launchApplet(Applet,Display);
        }
        //Testeo la colision con las paredes del escenario
        if(node->pos.x < 0 || node->pos.y < 0 || node->pos.x > Display->w || node->pos.y > Display->h )
            colision = true;

        //Avanozo al siguiente nodo y voy intercambiando las posiciones
        node = node->next;
        while(node){
            tmp = node->pos;
            node->pos = lastPos;
            lastPos = tmp;
            //Testeo la colision con la propia serpiente
            if((snake->pos.x == node->pos.x) && (snake->pos.y == node->pos.y))
                colision = true;

            node = node->next;
        }
        if(colision){
            SDL_UserEvent e;
            e.type = gameOverEventID;
            SDL_PushEvent((SDL_Event*)&e);
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


void initDrawableSnake(drawableSnake *d, snakeHead *head){
    if(d && head){
        d->snake = head;
        d->renderDrawable = &renderSnake;
    }
}
