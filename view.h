#ifndef VIEW_H
#define VIEW_H

#include "renderer.h"
#include "eventloop.h"

typedef struct view:renderer::drawable{
    event::EventLoop* loop;
    SDL_Rect r;
}view;

void initView(view* v);

#endif // VIEW_H
