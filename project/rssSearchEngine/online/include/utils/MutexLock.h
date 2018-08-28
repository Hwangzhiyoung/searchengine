 ///
 /// @file    MutexLock.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 11:07:13
 ///


#ifndef _WD_MUTEXLOCK_H
#define _WD_MUTEXLOCK_H

#include "Noncopyable.h"
#include <pthread.h>

namespace wd
{

class MutexLock : Noncopyable
{
public:
	MutexLock();
	~MutexLock();

	void lock();
	void unlock();
	
	pthread_mutex_t * getMutexLockPtr();


private:
	pthread_mutex_t mutex_;
	bool isLocked_;
};


class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex);
	~MutexLockGuard();

private:
	MutexLock & mutex_;
};

}// end of namespace wd

#endif
