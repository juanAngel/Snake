#include "guiview.h"

#include "SDL_ttf.h"

TTF_Font* font;
SDL_Color fontColor;
SDL_Color fontColorBacground;
SDL_Rect r = {200,200,200,60};

void windowsDraw(renderer::DrawContext* context,windowsView* drawableObject){
    printf("windowsDraw\n");

    //Intento renderizar el texto
    SDL_Surface* textSurface = TTF_RenderText(font,"Game Over",fontColor,fontColorBacground);
    if(textSurface){
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(context->renderer,textSurface);

        SDL_RenderCopy(context->renderer,textTexture,NULL,&r);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }else{
        printf("%s\n",TTF_GetError());
    }

}

void initWindows(windowsView *w, event::EventLoop *defaultLoop){
    if(w){
        char* fontName = "font/lazy.ttf";
        memset(w,0,sizeof(*w));
        initView(w,defaultLoop);
        w->renderDrawable = (renderer::DrawFunc)&windowsDraw;
        //Inicializo ttf
        TTF_Init();
        //Intento abrir las fuentes
        font = TTF_OpenFont(fontName,200);
        if(font){
            fontColor.r = fontColor.g = fontColor.b = fontColor.a = 0xff;
            fontColorBacground.r = fontColorBacground.g = fontColorBacground.b = fontColorBacground.a = 0;
        }else{
            printf("%s\n",TTF_GetError());
        }
    }
}


void initLabel(labelView *w, event::EventLoop *defaultLoop){
    if(w){
        memset(w,0,sizeof(*w));
        initView(w,defaultLoop);
    }
}


void initButton(buttonView *w, event::EventLoop *defaultLoop){
    if(w){
        memset(w,0,sizeof(*w));
        initView(w,defaultLoop);
    }

}


void initTextField(textFieldView *w, event::EventLoop *defaultLoop){
    if(w){
        memset(w,0,sizeof(*w));
        initView(w,defaultLoop);
    }

}
