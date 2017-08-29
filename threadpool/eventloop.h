#ifndef __EVENTLOOP_H_
#define __EVENTLOOP_H_

#include <pthread.h>

class Eventloop
{
public:
    Eventloop();
    ~Eventloop();

    void loop();
    bool isInLoopThread() const { return (pthread_equal(threadId_, pthread_self()) != 0); }

    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
public:
    static Eventloop* getEventloopOfCurrentThread();
private:
    void abortNotInLoopThread();
private:
    bool looping_;
    const pthread_t threadId_;
};

#endif