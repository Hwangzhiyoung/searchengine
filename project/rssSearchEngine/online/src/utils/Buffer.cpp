 ///
 /// @file    Buffer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 11:31:01
 ///

#include "Buffer.h"

namespace wd
{
Buffer::Buffer(size_t size)
	:	mutex_(),
		notFull_(mutex_),
		notEmpty_(mutex_),
		size_(size),
		flag_(true)
{
}

bool Buffer::full()
{
	return size_ == que_.size();
}

bool Buffer::empty()
{
	return 0 == que_.size();
}
	  
void Buffer::push(Task task)
{
	MutexLockGuard guard(mutex_);//局部对象
	while(full())//使用while是为了被异常唤醒
	{
		notFull_.wait();
	}
	que_.push(task);
	
	notEmpty_.notify();

}


Buffer::Task Buffer::pop()
{
	MutexLockGuard guard(mutex_);
	//mutex_.lock();

	while(empty() && flag_)
	{
		notEmpty_.wait();
	}

	if(flag_)
	{
		Task task = que_.front();
		que_.pop();

		notFull_.notify();
		//mutex_.unlock();
		return task;
	}
	else
	{
		//mutex_.unlock();
		return NULL;
	}
}


void Buffer::wakeup_empty()
{
	notEmpty_.notifyall();
}

void Buffer::set_flag(bool flag)
{
	flag_ = flag;
}

}//end of namespace wd
