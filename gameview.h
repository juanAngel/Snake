/** @file */
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "view.h"
#include "snake.h"
#include <SDL2/SDL_mixer.h>
/**
 * @brief Representa la vista del juego
 *
 * Contiene todos los elementos que intervienen en el juego
 */
typedef struct GameView:view{
    drawableSnake drawSnake;
    SDL_Point Applet;
    bool start;
    TTF_Font* defaultFont;
}GameView;
/**
 * @brief initGameView Inicializa la vista del juego
 * @param game Objeto GameView
 * @param r Retangulo de la vista
 * @param defaultLoop Manejador de eventos por defecto
 */
void initGameView(GameView* game, SDL_Rect *r, event::EventLoop *defaultLoop);


#endif // GAMEVIEW_H
