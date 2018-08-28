 ///
 /// @file    Threadpool.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-03 15:49:51
 ///


#ifndef __WD_THREADPOOL_H
#define __WD_THREADPOOL_H

#include "Buffer.h"
#include <vector>
#include <functional>

namespace wd
{
class Thread;

class Threadpool
{
public:
	typedef std::function<void()> Task;
	Threadpool(size_t threadsNum, size_t bufNum);
	~Threadpool();

	void start();
	void stop();

	void addTask(Task task);

private:
	void threadFunc();
	Task getTask();

private:
	size_t threadsNum_;
	std::vector<Thread *> vecThreads_;
	Buffer buf_;
	bool isExit_;
};

}// end of namespace wd

#endif
