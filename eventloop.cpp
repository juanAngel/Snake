#include "eventloop.h"

namespace event {
    void runLoop(EventLoop */*loop*/,bool* /*runing*/){

    }


    void setDispacher(EventLoop* loop, EventLoopDispacher d){
        if(loop && d){
            loop->d = d;
        }
    }

    void setContext(EventLoop *l, void *loopContext){
        if(l)
            l->loopContext = loopContext;
    }

    int dispachEvent(EventLoop *loop, SDL_Event *e){
        int status = EVENT_NOT_HANDLE;
        while (loop) {
            if((status = loop->d(loop,e)) == 0){
                loop = (event::EventLoop*)loop->next;
            }else{
                loop = nullptr;
            }
        }
        return status;
    }



}


