/** @file
 * @author  Juan Angel
*/
#ifndef VIEW_H
#define VIEW_H

#include "renderer.h"
#include "eventloop.h"

/**
 * @brief Representa la estructura basica de una vista\n
 *
 * Una vista basica es invisible si no se sobrescribe la funcion de pintado,
 * pero es capaz de recibir eventos.\n
 * Las vistas son las encargadas de presentar los datos e interactuar con el usuario
 * */
typedef struct view:renderer::drawable{
    event::EventLoop* loop; /**< Lista de manejadores de eventos de la vista */
    SDL_Rect r;/**< Rectangulo de la vista de la vista */
    SDL_Color backgroundColor; /**< Color de fondo de la vista */
}view;
/**
 * @brief initView inicializa una vista
 * @param v vista a inicializar
 * @param defaultLoop despachador de eventos por defecto
 */
void initView(view* v, event::EventLoop *defaultLoop);
/**
 * @brief runView Empieza a ejecutar la vista indicada en la variable view, y para cuando stop sea true
 * @param view vista a ejecutar
 * @param stop variable indicadora de parada
 */
void runView(view **view, bool* stop);
/**
 * @brief setPoss Cambia la posicion de la vista de la vista
 *
 * La posicion de la vista siempre es relativa a la vista padre. si no tiene padre es relativa a la ventana de la aplicaccion
 *
 * @param v Vista a la que se le cambia la posicion
 * @param p Nueva posicion de la esquina superior izquierda de  la vista
 */
void setPoss(view* v,SDL_Point p);
/**
 * @brief setSize Cambia el tamaño de la vista
 *
 * Las vistas hijas quedan encerradas en el tmaño de la vista padre. Si las hijas tienen un tamaño mayor que el padre
 * @param v Vista a la que se le cambia el tamaño
 * @param s Nuevo tamaño
 */
void setSize(view* v,Size s);

#endif // VIEW_H
