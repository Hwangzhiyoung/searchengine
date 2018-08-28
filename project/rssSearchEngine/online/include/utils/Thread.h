 ///
 /// @file    thread.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-10-31 10:21:35
 ///


#ifndef _MY_THREAD_H_
#define _MY_THREAD_H_

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>


namespace wd
{
class Thread : Noncopyable
{
public:
	typedef std::function<void()> ThreadCallback;
	Thread(ThreadCallback cb);
	~Thread();

	void start();
	void join();

	static void * threadFunc(void * arg);
private:
	pthread_t pthId_;
	bool isRunning_;
	ThreadCallback cb_;
};
}//end of namespace wd
#endif
