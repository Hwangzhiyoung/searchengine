 ///
 /// @file    Threadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-03 15:53:35
 ///

#include "Threadpool.h"
#include "Thread.h"
#include <stdio.h>

namespace wd
{
Threadpool::Threadpool(size_t threadsNum, size_t bufNum)
	: threadsNum_(threadsNum),
	  buf_(bufNum),
	  isExit_(false)
{
	printf("Threadpool()\n");
}

Threadpool::~Threadpool()
{
	if(!isExit_)
	{
		stop();
	}
}

void Threadpool::start()
{
	for(size_t idx = 0; idx != threadsNum_; ++idx)
	{
		Thread * pThread = new Thread(
			std::bind(&Threadpool::threadFunc, this));
		vecThreads_.push_back(pThread);
	}

	std::vector<Thread *>::iterator it;
	for(it = vecThreads_.begin(); it != vecThreads_.end(); ++it)
	{
		(*it)->start();
	}
	printf("Threadpool::start()\n");
}


void Threadpool::stop()
{
	if(!isExit_)
	{
		isExit_ = true;
		buf_.set_flag(false);
		buf_.wakeup_empty();

		std::vector<Thread *>::iterator it;
		for(it = vecThreads_.begin(); it != vecThreads_.end(); ++it)
		{
			(*it)->join();
			delete (*it);
		}
		vecThreads_.clear();
	}
}

void Threadpool::addTask(Task task)
{
	printf("Threadpool::addTask()\n");
	buf_.push(task);
}

Threadpool::Task Threadpool::getTask()
{
	return buf_.pop();
}

void Threadpool::threadFunc()
{
	while(!isExit_)
	{
		Task task = buf_.pop();
		if(task)
		{
			task();
		}
	}
}


}// end of namespace wd
