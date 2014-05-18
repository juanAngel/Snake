#include "view.h"


//funcion de draw que no hace nada
void viewNullDraw(renderer::DrawContext /*context*/,renderer::drawable* /*drawableObject*/){}

void initView(view *v,event::EventLoop* defaultLoop){
    if(v){
        v->renderDrawable = &viewNullDraw;

        v->loop = defaultLoop;
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
                        loop = (event::EventLoop*)loop->next;
                    }else break;
                }
            }
            if(noEvent){
                loop = (*view)->loop;
                while (loop) {
                    if(loop->d(loop,NULL) == 0){
                        loop = (event::EventLoop*)loop->next;
                    }else break;
                }
            }
        }
    }
}


void setPoss(view *v, SDL_Point p){
    if(v){
        v->r.x = p.x;
        v->r.y = p.y;
    }
}
void setSize(view *v, Size s){
    if(v && s.h && s.w){
        v->r.w = s.w;
        v->r.h = s.h;
    }
}
