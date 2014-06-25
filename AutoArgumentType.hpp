/***************************
//@ScopeGuard
//Author : AntiMoron
//Date   : 2014-06-26
***************************/

#ifndef __AUTOARGUMENTTYPE_H__
#define __AUTOARGUMENTTYPE_H__

#include<type_traits>

template<typename source_type>
class auto_argument
{
	template<typename temp_type, typename Enable = void>
	struct test;

	template<typename temp_type>
	struct test<temp_type,typename std::enable_if<std::is_destructible<temp_type>::value>::type >
	{
		typedef temp_type& type;
	};

	template<typename temp_type>
	struct test<temp_type,typename std::enable_if<!(std::is_destructible<temp_type>::value)>::type >
	{
		typedef temp_type type;
	};
public:
	typedef typename test<source_type>::type type;
};

#endif




///@test code as follows

//struct test
//{
//	test(int _ = -1){x = _;};
//	~test(){}
//	test(const test&){}	// disable default copy constructor
//	int x;
//};
//int foo(auto_argument<test>::type x)
//{
//	printf("%d\r\n",x.x);
//	return 0;
//}
//int regularFoo(test x)
//{
//	printf("%d\r\n",x.x);
//	return 0;
//}
//
//int main()
//{
//	test x;
//	x.x = 10;
//	foo(x);
//	regularFoo(x);
//	return 0;
//}
