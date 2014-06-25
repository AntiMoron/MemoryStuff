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

