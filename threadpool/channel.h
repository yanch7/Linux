#include "eventloop.h"
#include <functional>
/*
channel用于将文件描述符，此描述符上要监听的事件，相应的回调函数，以及事件循环绑定在一起。
*/
class Channel
{
public:
    typedef std::function<void ()> EventCallback;
    Channel(Eventloop* loop, int fd);
    void handleEvent();
    void setReadCallback(const EventCallback& cb)
    {
        readCallback_ = cb;
    }
    void setWriteCallback(const EventCallback& cb)
    {
        writeCallback_ = cb;
    }

    void setErrorCallback(const EventCallback& cb)
    {
        errorCallback_ = cb;
    }

    int fd() const { return fd_; }
    int events
private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
private:
    Eventloop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};