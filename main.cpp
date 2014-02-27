
#include "snake_global.h"
#include "renderer.h"
#include "scene.h"


uint updateEventID;
uint gameOverEventID;

typedef struct Context{
    //Indica si es necesario redibujar
    bool needRedraw;
    //Render y ventana
    SDL_Window* window;
    SDL_Renderer* renderer;
    //Guarda el recuadro de la pantalla
    SDL_Rect display;
    //Esto representa mi escena
    scene escena;
}Context;

void initContext(Context* c,int x,int y,int w,int h);


int main(int /*argc*/, char* /*argv*/[]){
    Context context;
    scene* escena = &context.escena;
    snakeHead* snake = &escena->sake;
    bool exitApp = false;
    bool gameOver = false;
    bool isSnakeStart = false;


    initContext(&context,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480);
    initScene(&context.display,&context.escena);
    updateEventID = SDL_RegisterEvents(2);
    gameOverEventID = updateEventID+1;

    SDL_Event e;
    while (!exitApp) {
        if(!context.needRedraw){
            bool noEvent = true;
            while(SDL_PollEvent(&e)){
                switch (e.type) {
                    case SDL_QUIT:
                        exitApp = true;
                        break;
                    debug(
                    case SDL_KEYUP:
                        printf("SDL_KEYUP\n");
                        break;
                    )
                    case SDL_KEYDOWN:
                        debug(printf("SDL_KEYDOWN\n");)
                        switch (e.key.keysym.sym) {
                            /*
                             * segun las teclas de flecha usadas,
                             * pone la direccionde la serpienta hacia un lado u otro
                             */
                            case SDLK_UP:
                                if(snake->vecDirector.y == 0 || !isSnakeStart)
                                    snake->vecDirector = direcUp;
                                break;
                            case SDLK_DOWN:
                                if(snake->vecDirector.y == 0 || !isSnakeStart)
                                    snake->vecDirector = direcDown;
                                break;
                            case SDLK_LEFT:
                                if(snake->vecDirector.x == 0 || !isSnakeStart)
                                    snake->vecDirector = direcLetf;
                                break;
                            case SDLK_RIGHT:
                                if(snake->vecDirector.x == 0 || !isSnakeStart)
                                    snake->vecDirector = direcRight;
                                break;
                        }
                        if(!isSnakeStart){
                            startSnake(snake,&context.display,&context.escena.Applet);
                            isSnakeStart = true;
                        }

                        context.needRedraw = true;
                        break;
                    case SDL_MOUSEMOTION:
                        if(e.motion.state&SDL_BUTTON_LMASK){
                            int xrel = e.motion.xrel;
                            int yrel = e.motion.yrel;

                            //Elimino las diagonales y normalizo los valores
                            if(abs(xrel) > abs(yrel)){
                                yrel = 0;
                                if(xrel)xrel /= abs(xrel);
                            }else{
                                xrel = 0;
                                if(yrel)yrel /= abs(yrel);
                            }
                            snake->vecDirector.x = xrel;
                            snake->vecDirector.y = yrel;

                            printf("mouse drop x=%d y=%d\n"
                                   ,xrel
                                   ,yrel);

                            if(!isSnakeStart){
                                startSnake(snake,&context.display,&context.escena.Applet);
                                isSnakeStart = true;
                            }

                            context.needRedraw = true;
                        }
                        break;
                    default:
                        if(updateEventID == e.type){
                            context.needRedraw = true;
                        }else if(gameOverEventID == e.type){
                            gameOver = true;
                            SDL_RemoveTimer(snake->timer);
                            printf("GameOver\n");
                        }/*else{
                            printf("unknown event %x\n",e.type);
                        }*/
                        break;
                }
                noEvent = false;
            }
            if(noEvent)
                    SDL_Delay(5);
        }else{
            drawScene(context.renderer,escena);
            context.needRedraw = false;
        };
    }
    SDL_Quit();
    return 0;
}

void initContext(Context* c, int x, int y, int w, int h){
    memset(c,0,sizeof(*c));

    c->display.x = x;
    c->display.y = y;
    c->display.w = w;
    c->display.h = h;

    c->needRedraw = true;

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
    c->renderer = SDL_CreateRenderer(c->window,0,SDL_RENDERER_ACCELERATED);

}
