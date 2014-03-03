#include "eventloop.h"

namespace event {
    void runLoop(EventLoop */*loop*/){

    }


    void setDispacher(EventLoop* loop, EventLoopDispacher *d){
        if(loop && d){
            loop->d = d;
        }
    }
}


