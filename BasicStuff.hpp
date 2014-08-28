/***************************
//@ScopeGuard
//Author : AntiMoron
//Date   : 2014-05-24
***************************/

#ifndef __BASICSTUFF_H__
#define __BASICSTUFF_H__

#include<type_traits>
#include<memory>
#include<functional>
#include<iostream>

namespace MSF
{
	typedef signed char CHAR;
	typedef unsigned char UCHAR;
	typedef unsigned char BYTE;
	typedef signed short SHORT;
	typedef unsigned short USHORT;
	typedef signed int INT;
	typedef unsigned int UINT;
	typedef signed long LONG;
	typedef unsigned long ULONG;
	typedef signed long long LL;
	typedef unsigned long long ULL;
	typedef float	FLOAT;
	typedef double 	DOUBLE;
	typedef long double LDOUBLE;
}

class ScopeGuard
{
public:
	~ScopeGuard()
	{
		if(foo == nullptr)
			return ;
		foo();
		return ;
	}
private:
	friend ScopeGuard make_guard(std::function<void()> );
	explicit ScopeGuard()
	{
		foo = nullptr;
	}
	const ScopeGuard& operator = (const ScopeGuard& other)
	{
		foo = other.foo;
		return *this;
	}
	std::function<void()> foo;
};

ScopeGuard make_guard(std::function<void()> foo_ptr)
{
	ScopeGuard result;
	result.foo = foo_ptr;
	return result;
}

#endif

