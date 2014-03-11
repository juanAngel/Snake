#include "eventloop.h"

namespace event {
    void runLoop(EventLoop *loop,bool* runing){

    }


    void setDispacher(EventLoop* loop, EventLoopDispacher d){
        if(loop && d){
            loop->d = d;
        }
    }

    void setContext(EventLoop *l, void *userContext){
        if(l)
            l->userContext = userContext;
    }

    void addEventLoopFirs(EventLoop *l, EventLoop *newLoop){

    }

    void addEventLoopLast(EventLoop *l, EventLoop *newLoop){

    }


}


