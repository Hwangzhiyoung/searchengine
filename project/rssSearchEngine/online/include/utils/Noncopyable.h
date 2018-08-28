 ///
 /// @file    Noncopyable.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-02 10:54:57
 ///


#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace wd
{

class Noncopyable
{
protected:
	Noncopyable(){}
private:
	Noncopyable(const Noncopyable & rhs);
	Noncopyable & operator=(const Noncopyable & rhs);
};

}//end of namespace wd
#endif
