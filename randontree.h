/** @file */
#ifndef RANDONTREE_H
#define RANDONTREE_H

#include "renderer.h"
/**
 * @brief Contiene la informacion necesaria para pintar un Bosque aleatorio
 */
typedef struct forestRandon{
    SDL_Rect *Display;
    Size gridSize;
    SDL_Color treeColor;
    SDL_Color backgrundColor;
}forestRandon;
/**
 * @brief drawForestRandon Dibuja un arbol aleatorio
 * @param d Contexto de dibujo
 * @param f Configuracion para el dibujo de el bosque
 */
void drawForestRandon(renderer::DrawContext d, forestRandon* f);


#endif // RANDONTREE_H
