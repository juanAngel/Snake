#include "gameview.h"

const char marcadorFormat[] = "Puntos %u";

int gameViewEventDispacher(event::EventLoop* loop,SDL_Event* e);

void GameViewDraw(renderer::DrawContext* context,GameView* game){
    if(context && game){

        SDL_SetRenderDrawColor(renderer::getRenderer(context),0xff,0xff,0xff,0xff);

        char buffText[sizeof(marcadorFormat)+10];
        sprintf(buffText,marcadorFormat,game->drawSnake.snake.nAppletMeals);
        renderer::drawText(context,game->defaultFont,buffText);

        game->drawSnake.renderDrawable(context,&game->drawSnake);
        drawApplet(context,&game->Applet);
    }
}


void initGameView(GameView *game,SDL_Rect* r,event::EventLoop* defaultLoop){
    if(game && r){
        int audio_rate = 22050;
        Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
        int audio_channels = 2;
        int audio_buffers = 4096;

        if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
            printf("No se pudo abrir el audio!\n");
            exit(1);
        }

        event::EventLoop* newLoop;
        memset(game,0,sizeof(*game));
        initView(game,defaultLoop);

        newLoop = (event::EventLoop*)malloc(sizeof(*newLoop));
        newLoop->next = nullptr;
        addNodeBefore(newLoop,game->loop);

        game->loop = newLoop;
        event::setContext(newLoop,game);
        event::setDispacher(game->loop,&gameViewEventDispacher);

        game->r = *r;

        game->renderDrawable = (renderer::DrawFunc)&GameViewDraw;
        initDrawableSnake(&game->drawSnake,NULL);

        game->drawSnake.snake.music  = Mix_LoadMUS("sound/snake.ogg");
        Mix_VolumeMusic(MIX_MAX_VOLUME);

        launchSnake(&game->drawSnake,r);
        launchApplet(&game->Applet,r);
    }
}

int gameViewEventDispacher(event::EventLoop* loop,SDL_Event* e){
    int result = 0;
    if(loop && e){
        GameView* gameView = (GameView*)loop->loopContext;
        Snake* snake = &gameView->drawSnake.snake;
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
