#include "gameview.h"


void GameViewDraw(renderer::DrawContext* context,GameView* game){
    if(context && game){
        game->snake.renderDrawable(context,&game->snake);
        drawApplet(context->renderer,&game->Applet);
    }
}


void initGameView(GameView *game,SDL_Rect* r){
    if(game && r){
        memset(game,0,sizeof(*game));
        initView(game);
        game->renderDrawable = (renderer::DrawFunc)&GameViewDraw;
        initDrawableSnake(&game->snake,NULL);
        launchSnake(&game->snake,r);
        launchApplet(&game->Applet,r);
    }
}
