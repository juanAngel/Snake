#ifndef RENDERER_H
#define RENDERER_H

#include "snake_global.h"

void drawScene(SDL_Renderer* renderer, struct scene *escena);

namespace renderer {
    typedef struct DrawContext{
        SDL_Renderer* renderer;
        SDL_Point offset;
    }DrawContext;

    typedef void (*DrawFunc)(DrawContext* context,struct drawable* drawableObject);
    typedef struct drawable{
        DrawFunc renderDrawable;
    }drawable;

    int initDrawContext(DrawContext* c,DrawContext* old = 0);
}


#endif // RENDERER_H
