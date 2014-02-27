#include "renderer.h"
#include "scene.h"

void drawRectangle(SDL_Renderer* renderer,rectangle* r);

void drawScene(SDL_Renderer* renderer,scene* escena){
    register SDL_Color const* backgroundColor = &escena->backgroundColor;

    //Borramos la pantalla
    SDL_SetRenderDrawColor(renderer, backgroundColor->r,backgroundColor->g,backgroundColor->b,backgroundColor->a);
    SDL_RenderClear(renderer);

    //drawRectangle(renderer,&escena->r);
    drawSnake(renderer,&escena->sake);
    drawApplet(renderer,&escena->Applet);

    SDL_RenderPresent(renderer);
}


void drawRectangle(SDL_Renderer* renderer,rectangle* r){
    register SDL_Color const* fillColor = &r->fillColor;
    register SDL_Color const* borderColor = &r->borderColor;

    SDL_SetRenderDrawColor(renderer, fillColor->r,fillColor->g, fillColor->b, fillColor->a);
    SDL_RenderFillRect(renderer,r);
    SDL_SetRenderDrawColor(renderer, borderColor->r,borderColor->g, borderColor->b, borderColor->a);
    SDL_RenderDrawRect(renderer,r);
}


int renderer::initDrawContext(renderer::DrawContext *c, renderer::DrawContext *old){

}
