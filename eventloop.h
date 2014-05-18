/** @file
*
*/
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "snake_global.h"
#include "containerTypes.h"
/**
 * Evento no manejado
 */
#define EVENT_NOT_HANDLE    0
/**
 * Evento manejado
 */
#define EVENT_HANDLE        1
/**
 * Error al manejar el evento
 */
#define EVENT_ERROR         -1

namespace event {

    /**
    * Return 0 if not hantled
    * -1 at error
    * 1 handled
    */
    /**
     * @brief Puntero a funcion para el manejo de los eventos
     *
     * loop Manejador de eventos actual\n
     * e evento a manejar
     */
    typedef int (*EventLoopDispacher)(struct EventLoop* loop,SDL_Event* e);
    /**
     * @brief Una lista de despachadores de eventos para manejar los Eventos del bucle de eventos
     * */
    typedef struct EventLoop:linkList{
        EventLoopDispacher d;/**< Funcion encargada de despachar eventos */
        void* loopContext;/**< Puntero para uso de el despachador de eventor*/
    }EventLoop;
    /**
     * @brief runLoop Ejecuta un bucle de eventos y los despacha con el objeto l
     * @param l Lista de manejadores de eventos
     * @param runing Variable que indica cuando detener el bucle de eventos
     */
    void runLoop(EventLoop *l, bool *runing);
    /**
     * @brief dispachEvent Despacha un evento
     * @param loop Lista de manejadores de eventos
     * @param e Evento a despachar
     * @return EVENT_NOT_HANDLE si la cadena de eventos no manejo el evento, EVENT_HANDLE si alguno de
     * los manejadores manejo el evento y devolvio EVENT_HANDLE
     * y  EVENT_ERROR_HANDLE si se produjo algun error
     */
    int dispachEvent(struct EventLoop* loop,SDL_Event* e);
    /**
     * @brief setDispacher
     * @param loop Lista de manejadores de eventos
     * @param d Nueva funcion encargada de manejar los eventos en este EventLoop
     */
    void setDispacher(EventLoop* loop, EventLoopDispacher d);
    /**
     * @brief setContext Setea el userContext del EventLoop dado en l
     * @param l Lista de manejadores de eventos
     * @param loopContext
     */
    void setContext(EventLoop* l,void* loopContext);
    /**
     * @brief addEventLoopFirs Agrega un despachador de eventos al principio de la lista
     * @param l Lista de EventLoop
     * @param newLoop el EventLoop que se pomdra al principio
     */
    void addEventLoopFirs(EventLoop* l,EventLoop* newLoop);
    /**
     * @brief addEventLoopLast Agrega un despachador de eventos al final de la lista
     * @param l  Lista de EventLoop
     * @param newLoop el EventLoop que se pomdra al final
     */
    void addEventLoopLast(EventLoop* l,EventLoop* newLoop);
}


#endif // EVENTLOOP_H
