#include "view.h"

//funcion de draw que no hace nada
void viewNullDraw(renderer::DrawContext* /*context*/,renderer::drawable* /*drawableObject*/){}
void viewDefaultEventDispacher(event::EventLoop* loop,SDL_Event* e){}

void initView(view *v){
    if(v){
        event::EventLoop* newLoop;
        v->renderDrawable = &viewNullDraw;

        newLoop = (event::EventLoop*)malloc(sizeof(*newLoop));

        v->loop = newLoop;
        event::setContext(newLoop,v);
        event::setDispacher(newLoop,&viewDefaultEventDispacher);
    }
}
