#ifndef VIEW_H
#define VIEW_H

#include "renderer.h"
#include "eventloop.h"

typedef struct view:renderer::drawable{
    event::EventLoop* loop;
    SDL_Rect r;
    SDL_Color backgroundColor;
}view;

void initView(view* v, event::EventLoop *defaultLoop);
void runView(view **view, bool* stop);

#endif // VIEW_H
