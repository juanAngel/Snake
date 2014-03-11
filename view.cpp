#include "view.h"

//funcion de draw que no hace nada
void viewNullDraw(renderer::DrawContext* /*context*/,renderer::drawable* /*drawableObject*/){}
int viewDefaultEventDispacher(event::EventLoop* loop,SDL_Event* e){
    return 0;
}

void initView(view *v,event::EventLoop* defaultLoop){
    if(v){
        event::EventLoop* newLoop;
        v->renderDrawable = &viewNullDraw;

        newLoop = (event::EventLoop*)malloc(sizeof(*newLoop));

        v->loop = newLoop;
        newLoop->next = defaultLoop;
        event::setContext(newLoop,v);
        event::setDispacher(newLoop,&viewDefaultEventDispacher);
    }
}
void runView(view **view, bool* stop){
    if(view && stop){
        SDL_Event e;
        bool noEvent;
        event::EventLoop* loop;
        while (!*stop) {
            noEvent = true;
            while(SDL_PollEvent(&e)){
                noEvent = false;
                loop = (*view)->loop;
                while (loop) {
                    if(loop->d(loop,&e) == 0){
                        loop = loop->next;
                    }else break;
                }
            }
            if(noEvent){
                loop = (*view)->loop;
                while (loop) {
                    if(loop->d(loop,NULL) == 0){
                        loop = loop->next;
                    }else break;
                }
            }
        }
    }
}
