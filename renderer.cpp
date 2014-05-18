#include "renderer.h"


typedef struct DrawContextPrivate{
    SDL_Renderer* renderer;
    SDL_Point offset;
    SDL_Color clearColor;
    SDL_Texture* backgroundImg;
    bool needRedraw;
}DrawContextPrivate;

TTF_Font* g_defaultFont;
SDL_Color g_defaultFontColor = {0,0,0,0xff};

void renderer::drawText(renderer::DrawContext c,TTF_Font* font,const char* text){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(!font)
        font = g_defaultFont;

    if(text && font){
        SDL_Color color;
        SDL_GetRenderDrawColor(context->renderer,&color.r,&color.g,&color.b,&color.a);
        //Intento renderizar el texto
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font,text,color);
        if(textSurface){
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(context->renderer,textSurface);

            SDL_Rect rect;
            rect.x = context->offset.x;
            rect.y = context->offset.y;
            rect.h = textSurface->h;
            rect.w = textSurface->w;
            SDL_RenderCopy(context->renderer,textTexture,NULL,&rect);

            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }else{
            printf("%s\n",TTF_GetError());
        }
    }
}
renderer::DrawContext renderer::createDrawContext(){
    DrawContextPrivate* c = (DrawContextPrivate*)malloc(sizeof(DrawContextPrivate));
    memset(c,0,sizeof(DrawContextPrivate));

    return c;
}
/*
void renderer::copyDrawContext(renderer::DrawContext c, renderer::DrawContext o){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    DrawContextPrivate* old = (DrawContextPrivate*)o;
    if(context && old){
        context->backgroundImg = nullptr;
        context->clearColor = old->clearColor;
        context->needRedraw = old->needRedraw;
        context->offset = old->offset;
        context->renderer = old->renderer;
    }
    return 0;
}*/


void renderer::draw(renderer::DrawContext c, renderer::drawable *d){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(context){
        renderer::clear(context);
        if(d)
            d->renderDrawable(context,d);
        SDL_RenderPresent(context->renderer);
    }
}


void renderer::clear(DrawContext c){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(context){
        if(!context->backgroundImg){
            SDL_SetRenderDrawColor(context->renderer,
                                   context->clearColor.r,// red
                                   context->clearColor.g,// blue
                                   context->clearColor.b,// green
                                   context->clearColor.a);// alpha
            SDL_RenderClear(context->renderer);
        }else{
            SDL_RenderCopy(context->renderer,context->backgroundImg,nullptr,nullptr);
        }
    }
}


void renderer::sendUpdateWindows(){
    SDL_Event e;
    e.type = updateEventID;

    SDL_PushEvent(&e);
}


SDL_Texture *renderer::screenshot(SDL_Window* w){
    SDL_Texture* img = nullptr;

    SDL_Surface* surface = SDL_GetWindowSurface(w);
    SDL_Renderer* renderer = SDL_GetRenderer(w);
    if(surface && renderer){
        SDL_RenderReadPixels(renderer,&surface->clip_rect,surface->format->format
                             ,surface->pixels,surface->pitch);
        img = SDL_CreateTextureFromSurface(renderer,surface);
    }

    return img;
}
void textDraw(renderer::DrawContext c,renderer::drawableText* t){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(t && t->text){
        SDL_SetRenderDrawColor(context->renderer,t->textColor.r,t->textColor.g,t->textColor.b,t->textColor.a);
        renderer::drawText(context,t->font,t->text);
    }
}

void renderer::initDrawableText(renderer::drawableText *dText, TTF_Font *font){
    if(dText){
        dText->renderDrawable = (DrawFunc)&textDraw;
        dText->text = nullptr;
        dText->font = font;
        dText->textColor = {0,0,0,0xff};
    }
}


void renderer::fillRect(DrawContext c, SDL_Rect *r, SDL_Color* color){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(color->a){
        SDL_Rect rectangle = *r;
        rectangle.x += context->offset.x;
        rectangle.y += context->offset.y;
        SDL_SetRenderDrawColor(context->renderer,color->r,color->g,color->b,color->a);
        SDL_RenderFillRect(context->renderer,&rectangle);
    }
}


void renderer::drawRect(renderer::DrawContext c, SDL_Rect *r, SDL_Color *color){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    if(color->a){
        SDL_Rect rectangle = *r;
        rectangle.x += context->offset.x;
        rectangle.y += context->offset.y;
        SDL_SetRenderDrawColor(context->renderer,color->r,color->g,color->b,0);
        SDL_RenderDrawRect(context->renderer,&rectangle);
    }
}


SDL_Rect renderer::clipRect(renderer::DrawContext c){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    SDL_Rect r;
    SDL_RenderGetClipRect(context->renderer,&r);
    return r;
}


void renderer::setClipRect(renderer::DrawContext c, SDL_Rect *r){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    SDL_RenderSetClipRect(context->renderer,r);
}


void renderer::setBackgroundImg(renderer::DrawContext context, SDL_Texture *img){
    DrawContextPrivate* c = (DrawContextPrivate*)context;
    c->backgroundImg = img;
}


void renderer::setRenderer(renderer::DrawContext c, SDL_Renderer *r){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    context->renderer = r;
}


SDL_Renderer *renderer::getRenderer(renderer::DrawContext context){
    DrawContextPrivate* c = (DrawContextPrivate*)context;
    return c->renderer;
}


void renderer::setOffsetPoint(renderer::DrawContext c, SDL_Point *p){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    context->offset = *p;
}


SDL_Point renderer::offsetPoint(renderer::DrawContext c){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    return context->offset;
}


void renderer::offsetPointAdd(renderer::DrawContext c, SDL_Point *p){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    context->offset.x += p->x;
    context->offset.y += p->y;
}


void renderer::drawLine(renderer::DrawContext c, SDL_Point *p1, SDL_Point *p2, SDL_Color *color){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    SDL_SetRenderDrawColor(context->renderer,color->r,color->g,color->b,color->a);
    SDL_RenderDrawLine(context->renderer,p1->x+context->offset.x,p1->y+context->offset.y
                       ,p2->x+context->offset.x,p2->y+context->offset.y);
}


void renderer::setClearColor(renderer::DrawContext c, SDL_Color *color){
    register DrawContextPrivate* context = (DrawContextPrivate*)c;
    context->clearColor = *color;
}


int renderer::getHeigthFont(TTF_Font *font){
    return TTF_FontLineSkip(font);
}

TTF_Font *renderer::defaultFont(){
    return g_defaultFont;
}


void renderer::setDefaultFont(TTF_Font *font){
    g_defaultFont = font;
}
