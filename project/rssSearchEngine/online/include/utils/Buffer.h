 ///
 /// @file    Buffer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 11:24:53
 ///


#ifndef _WD_BUFFER_H
#define _WD_BUFFER_H 

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>
#include <functional>

namespace wd
{

class Buffer : Noncopyable
{
public:
	typedef std::function<void()> Task;
	Buffer(size_t size);

	void push(Task task);
	Task pop();

	bool full();
	bool empty();

	void wakeup_empty();
	void set_flag(bool flag);

private:
	MutexLock          mutex_;
	Condition          notFull_;
	Condition          notEmpty_;
	size_t             size_;
	std::queue<Task>  que_;
	bool               flag_;
};

}// end of namespace wd

#endif
