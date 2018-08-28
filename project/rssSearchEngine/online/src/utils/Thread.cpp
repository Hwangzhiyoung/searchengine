 ///
 /// @file    thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-10-31 10:23:55
 ///


#include "Thread.h"
#include <iostream>

namespace wd
{

Thread::Thread(ThreadCallback cb) 
	: pthId_(0),
	  isRunning_(false),
	  cb_(cb)
{
	std::cout << "Thread()" << std::endl;
}

void Thread::start()
{
	pthread_create(&pthId_, NULL, threadFunc, this);
	isRunning_ = true;
}

void Thread::join()
{
	pthread_join(pthId_, NULL);
	isRunning_ = false;
}

void* Thread::threadFunc(void * arg)
{
	Thread * pThread = static_cast<Thread *>(arg);
	if(pThread != NULL)
	{	
		pThread->cb_();
		return pThread;
	}
	return NULL;
	
}

Thread::~Thread()
{
	if(isRunning_)
		pthread_detach(pthId_);
	std::cout << "~Thread()" << std::endl;
}

} // end of namespace wd
