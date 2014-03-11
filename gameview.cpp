#include "gameview.h"

int gameViewEventDispacher(event::EventLoop* loop,SDL_Event* e);

void GameViewDraw(renderer::DrawContext* context,GameView* game){
    if(context && game){
        game->snake.renderDrawable(context,&game->snake);
        drawApplet(context->renderer,&game->Applet);
    }
}


void initGameView(GameView *game,SDL_Rect* r,event::EventLoop* defaultLoop){
    if(game && r){
        memset(game,0,sizeof(*game));
        initView(game,defaultLoop);
        game->r = *r;

        game->renderDrawable = (renderer::DrawFunc)&GameViewDraw;
        game->loop->d = &gameViewEventDispacher;
        initDrawableSnake(&game->snake,NULL);
        launchSnake(&game->snake,r);
        launchApplet(&game->Applet,r);
    }
}

int gameViewEventDispacher(event::EventLoop* loop,SDL_Event* e){
    int result = 0;
    if(loop && e){
        GameView* gameView = (GameView*)loop->userContext;
        snakeHead* snake = &gameView->snake.head;
        switch (e->type) {
            case SDL_KEYDOWN:
                result++;
                switch (e->key.keysym.sym) {
                    /*
                     * segun las teclas de flecha usadas,
                     * pone la direccionde la serpienta hacia un lado u otro
                     */
                    case SDLK_UP:
                        if(snake->vecDirector.y == 0 || !gameView->start)
                            snake->vecDirector = direcUp;
                        break;
                    case SDLK_DOWN:
                        if(snake->vecDirector.y == 0 || !gameView->start)
                            snake->vecDirector = direcDown;
                        break;
                    case SDLK_LEFT:
                        if(snake->vecDirector.x == 0 || !gameView->start)
                            snake->vecDirector = direcLetf;
                        break;
                    case SDLK_RIGHT:
                        if(snake->vecDirector.x == 0 || !gameView->start)
                            snake->vecDirector = direcRight;
                        break;
                }
                if(!gameView->start){
                    startSnake(snake,&gameView->r,&gameView->Applet);
                    gameView->start = true;
                }
                break;
            case SDL_MOUSEMOTION:
                if(e->motion.state&SDL_BUTTON_LMASK){
                    int xrel = e->motion.xrel;
                    int yrel = e->motion.yrel;

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

                    if(!gameView->start){
                        startSnake(snake,&gameView->r,&gameView->Applet);
                        gameView->start = true;
                    }
                }
                break;
            default:
                break;
        }
    }
    return result;
}
