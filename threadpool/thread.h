#ifndef __THREAD_H_
#define __THREAD_H_

#include <pthread.h>
#include <functional>
#include <string>
#include <atomic>
#include <sys/types.h>
#include <sys/unistd.h>
#include "nocopyable.h"

class Thread : nocopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();

	void start();
	int join();
	bool started() const { return started_; }
private:
	void setDefaultName();

	bool started_;
	bool joined_;
	pthread_t tid_;
	std::string name_;
	ThreadFunc func_;

	static std::atomic_int numCreated_;
};

#endif
