#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "snake_global.h"

namespace event {

    /*
    * Return 0 if not hantled
    */
    typedef int (*EventLoopDispacher)(struct EventLoop* loop,SDL_Event* e);

    typedef struct EventLoop{
        EventLoop* next;
        EventLoopDispacher d;
        void* userContext;
    }EventLoop;

    void runLoop(EventLoop *l, bool *runing);
    void setDispacher(EventLoop* loop, EventLoopDispacher d);
    void setContext(EventLoop* l,void* userContext);

    void addEventLoopFirs(EventLoop* l,EventLoop* newLoop);
    void addEventLoopLast(EventLoop* l,EventLoop* newLoop);

}


#endif // EVENTLOOP_H
