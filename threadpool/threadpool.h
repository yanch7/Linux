#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include <functional>
#include <string>
#include <queue>
#include <memory>
#include <vector>
#include <algorithm>
#include "thread.h"
#include "mutex.h"
#include "condition.h"

/*
Threadpool类 ： 线程池
1. 线程
线程池使用Thread类来创建线程，使用std::shared_ptr指针管理Thread对象，
并且将这些指针放到统一的容器中。线程池中线程的数量在调用start()启动线程池时指定。
线程池的析构函数负责回收所有线程。
2. 任务队列
queue_是任务队列。任务队列通过mutex_进行保护，实现互斥访问。
3. 运行机制
调用start()启动线程池后，首先会根据start中的参数，创建相应数量的线程。
调用者可以通过run()函数将任务投入到任务队列中。如果队列已满，调用线程将等待在notFull_上。
在线程池内部，线程会尝试从任务队列中抓取任务。如果任务队列已空，线程将等待在notEmpty_上。
否则就在线程中执行所获取的任务。
*/
typedef std::function<void()> Task;

class Threadpool : nocopyable
{
public:
    explicit Threadpool(const std::string& name = std::string("threadpool"));
    ~Threadpool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(const Task& cb) { threadInitCallback_ = cb; }

    void start(int numThreads);
    void stop();

    const std::string& name() const { return name_; }
    size_t queueSize() const;

    void run(const Task&);
private:
    bool isFull() const;
    void runInThread();
    Task take();
private:
    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    std::string name_;
    Task threadInitCallback_;
    size_t maxQueueSize_;
    bool running_;
    std::vector<std::shared_ptr<Thread>> threads_;
    
    std::queue<Task> queue_;
};

#endif