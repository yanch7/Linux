#include "eventloop.h"
#include <cstdlib>
#include <cstdio>
#include <poll.h>
#include <cassert>

__thread Eventloop* t_loopInThisThread = nullptr;

Eventloop::Eventloop() : looping_(false), threadId_(pthread_self())
{
    if(t_loopInThisThread)
    {
        fprintf(stderr, "Another Eventloop exists\n");
        exit(1);
    }
    else t_loopInThisThread = this;
}

Eventloop::~Eventloop()
{
    looping_ = false;
    t_loopInThisThread = nullptr;
}

void Eventloop::abortNotInLoopThread()
{
    fprintf(stderr, "not in loop thread\n");
    exit(1);
}

Eventloop* Eventloop::getEventloopOfCurrentThread()
{
    return t_loopInThisThread;
}

void Eventloop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;

    ::poll(NULL, 0, 5 * 1000);

    looping_ = false;
}