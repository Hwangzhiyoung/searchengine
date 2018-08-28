 ///
 /// @file    MutexLock.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 11:12:01
 ///

#include "MutexLock.h"

namespace wd
{

MutexLock::MutexLock()
	: isLocked_(false)
{
	pthread_mutex_init(&mutex_, NULL);
}


MutexLock::~MutexLock()
{
	if(!isLocked_)
		pthread_mutex_destroy(&mutex_);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&mutex_);
	isLocked_ = true;
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&mutex_);
	isLocked_ = false;
}

pthread_mutex_t * MutexLock::getMutexLockPtr()
{
	return &mutex_;
}

MutexLockGuard::MutexLockGuard(MutexLock & mutex)
	: mutex_(mutex)
{
	mutex_.lock();
}

MutexLockGuard::~MutexLockGuard()
{
	mutex_.unlock();
}

}//end of namespace wd
