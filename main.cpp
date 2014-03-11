

#include "snake_global.h"
#include "renderer.h"
#include "scene.h"

#include "gameview.h"
#include "guiview.h"

#include <SDL/SDL_ttf.h>

uint updateEventID;
uint gameOverEventID;

typedef struct Context{
    //Indica si es necesario redibujar
    bool needRedraw;
    //Render y ventana
    SDL_Window* window;
    //SDL_Renderer* renderer;
    //Guarda el recuadro de la pantalla
    SDL_Rect display;
    //Esto representa mi escena
    scene escena;
    //Draw
    renderer::DrawContext drawContext;
    GameView* gameView;
    GuiView* guiView;
    view* currentView;
    event::EventLoop defaultLoop;
    bool exitApp;
}Context;

void initContext(Context* c,int x,int y,int w,int h);
void uninitContext(Context* c);
int DefaultDispacher(event::EventLoop *loop, SDL_Event *e);


int main(int /*argc*/, char* /*argv*/[]){
    Context context;
    scene* escena = &context.escena;
    snakeHead* snake = &escena->sake;

    //Inicializo el contexto
    initContext(&context,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480);

    //Configuro la serpiente para un paso de 5 y direccion arriba
    snake = &context.gameView->snake.head;
    snake->speed = snakeSize;
    snake->vecDirector = direcUp;

    //Registro los eventos
    updateEventID = SDL_RegisterEvents(2);
    gameOverEventID = updateEventID+1;

    //Pongo la vista por defecto
    context.currentView = context.gameView;

    //Arranco la vista
    runView(&context.currentView,&context.exitApp);

    uninitContext(&context);
    return 0;
}

void initContext(Context* c, int x, int y, int w, int h){
    memset(c,0,sizeof(*c));

    c->display.x = x;
    c->display.y = y;
    c->display.w = w;
    c->display.h = h;

    c->needRedraw = true;
    c->exitApp = false;
    //Inicializo el defalutLoop
    c->defaultLoop.d = &DefaultDispacher;
    c->defaultLoop.next = nullptr;
    c->defaultLoop.userContext = c;

    //TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);


    c->window = SDL_CreateWindow(
            "An SDL2 window",                  // window title
            x,           // initial x position
            y,           // initial y position
            w,                               // width, in pixels
            h,                               // height, in pixels
            SDL_WINDOW_OPENGL                  // flags - see below
        );
    SDL_GetWindowSize(c->window,&c->display.w,&c->display.h);
    SDL_GetWindowSize(c->window,&c->display.w,&c->display.h);
    //Creo una vista de juego
    c->gameView = (GameView*)malloc(sizeof(*c->gameView));
    initGameView(c->gameView,&c->display,&c->defaultLoop);

    windowsView* windows = (windowsView*)malloc(sizeof(*windows));
    initWindows(windows,&c->defaultLoop);
    c->guiView = windows;

    //Agregamos al final el manejador de eventos por defecto

    //initGameView(c->gameView,&c->display);

    c->drawContext.renderer = SDL_CreateRenderer(c->window,0,SDL_RENDERER_ACCELERATED);

}
void uninitContext(Context* c){
    if(c){
        if(c->drawContext.renderer)
            SDL_DestroyRenderer(c->drawContext.renderer);
        if(c->window)
            SDL_DestroyWindow(c->window);
    }
    SDL_Quit();
}
int DefaultDispacher(event::EventLoop *loop, SDL_Event *e){
    int result = 0;
    if(loop){
        Context* context = (Context*)loop->userContext;
        //Seteamos como manejado
        result++;
        if(e){
            //Manejo de los eventos por defecto
            switch (e->type){
                case SDL_QUIT:
                    context->exitApp = true;
                    break;
                default:
                    if(updateEventID == e->type){
                        context->needRedraw = true;
                    }else if(gameOverEventID == e->type){
                        //xcgameOver = true;
                        SDL_RemoveTimer(context->gameView->snake.head.timer);
                        context->needRedraw = true;
                        context->currentView = context->guiView;
                        printf("GameOver\n");
                    }
            }
        }else{
            //si no hay eventos entramos en modo IDL
            if(context->needRedraw){
                renderer::draw(&context->drawContext,context->currentView);
                context->needRedraw = false;
            }else{
                SDL_Delay(5);
            }
        }

    }
    return result;
}
