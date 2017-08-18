#include "thread.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <exception>

namespace detail
{
	struct ThreadData
	{
		typedef Thread::ThreadFunc ThreadFunc;
		ThreadFunc func_;
		std::string name_;

		ThreadData(const ThreadFunc& func, const std::string& name) : func_(func),
			name_(name) {}
		
		void runInThread()
		{
			try
			{
				func_();
			}
			catch(const std::exception& ex) 
			{
				fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
				fprintf(stderr, "reason: %s\n", ex.what());
				exit(1);
			}
		}
	};

	void* startThread(void *obj)
	{
		ThreadData* data = static_cast<ThreadData*>(obj);
		data->runInThread();
		delete data;
		return nullptr;
	}
}

std::atomic_int Thread::numCreated_(0);

Thread::Thread(const ThreadFunc& func, const std::string & name) : started_(false),
	joined_(false), func_(func), name_(name)
{
	setDefaultName();
}

Thread::~Thread()
{
	if(started_ && !joined_) 
	{
		pthread_detach(tid_);
	}
}

void Thread::setDefaultName()
{
	int num = numCreated_;
	if (name_.empty()) 
	{
		char buf[32];
		sprintf(buf, "Thread%d", num);
	}
}

int Thread::join()
{
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(tid_, NULL);
}

void Thread::start()
{
	assert(!started_);
	started_ = true;
	detail::ThreadData* data = new detail::ThreadData(func_, name_);
	if(pthread_create(&tid_, NULL, &detail::startThread, (void *)data))
	{
		started_ = false;
		delete data;
		exit(1);
	}
}
