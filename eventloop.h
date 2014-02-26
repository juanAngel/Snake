#ifndef EVENTLOOP_H
#define EVENTLOOP_H

typedef struct EventLoop{
    //dispacher;
    EventLoop* next;
}EventLoop;

void runEventLoop(EventLoop* l);
//void setDispacher(EventLoop* l,void(*)(EventLoop*,Event));

#endif // EVENTLOOP_H
