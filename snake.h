/** @file */
#ifndef SNAKE_H
#define SNAKE_H

#include "snake_global.h"
#include "renderer.h"
#include <SDL2/SDL_mixer.h>

//Estructura usafa para la deteccion de colisiones con la serpiente
typedef struct MapField{
    Size s;
    byte* buff;
}MapField;

/**
 * @brief Representa un nodo de la cola de la serpiente
 */
typedef struct snakeNode{
    snakeNode* next;
    SDL_Point pos;
}snakeNode;

/**
* @brief Representa la serpiente
*/
typedef struct Snake{
    snakeNode* head;/**< Principio de la cola de la serpiente */
    snakeNode* tail;/**< Final de la cola de la serpiente */
    SDL_Point vecDirector;/**< Direccion de avance de la serpiente la serpiente */
    double speed;/**< Numero de pixel que avanza la serpiente */
    int delay;/**< Retardo entre movimientos de la serpiente */
    SDL_TimerID timer;
    int nAppletMeals;/**< Numero de Manzanas comidas por la serpiente */
    MapField field;/**< Representa el campo por donde se mueve la serpiente */
    Mix_Music* music;
}Snake;

typedef struct drawableSnake: renderer::drawable{
    Snake snake;
}drawableSnake;
/**
 * @brief initDrawableSnake Inicializa un objeto serpiente pintable
 * @param d drawableSnake a inicializar
 * @param snake Objeto serpiente
 */
void initDrawableSnake(drawableSnake* d, Snake *snake);
/**
*  Recupera el numero de manzanas comidas
**/
/**
 * @brief getNAppletMeals Obtiene el numero de manzanas comidas
 * @param d Objeto serpiente pintable
 * @return Numero de manzanas comidas
 */
int getNAppletMeals(drawableSnake* d);

//Funciones para pintado de la serpiente y la manzana
/**
 * @brief drawApplet Pinta la manzana en la posicion indicada
 * @param context Contexto de dibujado
 * @param AppletPos Posicion a partir de donde se pinta la manzana
 */
void drawApplet(renderer::DrawContext context, SDL_Point* AppletPos);

//Manejo de la serpiente
/**
 * @brief moveSnake Mueve la serpiente
 * @param snake Serpiente a mover
 * @param Display Dimensiones de el campo
 * @param Applet Manzana
 * @note la serpiente se mueve al reves, la parte de la cola es la que va avanzando,
 * y los nodos que se quedan en la cabeza se ponen en la cola
 */
void moveSnake(Snake *snake, SDL_Rect *Display, SDL_Point *Applet);
/**
 * @brief startSnake Inicia el movimiento de la serpiente
 * @param snake Objeto serpiente
 * @param display Dimensiones del campo
 * @param applet Manzana
 */
void startSnake(Snake *snake, SDL_Rect *display, SDL_Point *applet);
/**
 * @brief resetSnake Resetea a el estado inicial la serpiente
 * @param drawable Objeto a resetear
 * @param display Valores de el tamaño del campo
 */
void resetSnake(drawableSnake* drawable,SDL_Rect* display);
/**
 * @brief addNodeHead Añade un nodo por la cabeza de la serpiente
 * @param snake Objeto serpiente
 */
void addNodeHead(Snake* snake);
/**
 * @brief addNodeTail Añade un nodo por la cola
 * @param snake Objeto serpiente al que se le añade un nodo
 */
void addNodeTail(Snake* snake);
/**
 * @brief moveHeadToTail Mueve el nodo de la cabeza y lo pone en la cola
 * @param snake  Objeto serpiente
 */
void moveHeadToTail(Snake *snake);
/**
 * @brief removeAllSnakeNodes Borra todos los nodos de la serpiente
 * @param snake Objeto serpiente al que al que se le eliminan los nodos
 */
void removeAllSnakeNodes(Snake *snake);
//Manejo de la manzana
/**
 * @brief launchApplet Pone una manzana en un lugar aleatorio de la pantalla
 * @param Applet    Puntero a un objeto punto donde se situaran las coordenadas de la manzana
 * @param display   Rectangulo de la pantalla
 */
void launchApplet(SDL_Point* Applet,SDL_Rect* display);
/**
 * @brief launchSnake Lanza la serpiente al campo
 * @param drawable  Objeto serpiente pintable
 * @param display   Rectangulo de la pantalla
 */
void launchSnake(drawableSnake* drawable,SDL_Rect* display);

//Constantes que sirven para setear el vector director en una direccion concreta
extern const SDL_Point direcUp,direcDown
                        ,direcRight,direcLetf;

#endif // SNAKE_H
