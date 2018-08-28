 ///
 /// @file    Condition.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 11:20:32
 ///


#include "Condition.h"
#include "MutexLock.h"


namespace wd
{

Condition::Condition(MutexLock & mutex)
	: mutex_(mutex)
{
	pthread_cond_init(&cond_, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&cond_);
}

void Condition::wait()
{
	pthread_cond_wait(&cond_, mutex_.getMutexLockPtr());
}

void Condition::notify()
{
	pthread_cond_signal(&cond_);
}

void Condition::notifyall()
{
	pthread_cond_broadcast(&cond_);
}


}//end of namespace wd
