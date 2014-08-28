/***************************
//@love letter
//Author : AntiMoron
//Date   : 2014-05-24
***************************/

#include"BasicStuff.hpp"
#include"AutoArgumentType.hpp"
#include"mp.hpp"
#include<iostream>
#include<stdio.h>

struct you
{
	int l;
	bool LoveMe;
	char* troubles;
};

int main()
{
	MemoryPool mp;
	you* p = (you*)mp.MPMalloc(sizeof(you));
    int* p2 = (int*)mp.MPMalloc(sizeof(int));
    short* p3 = (short*)mp.MPMalloc(sizeof(short));
    you* p4 = (you*)mp.MPMalloc(sizeof(you));
    mp.Display();
    mp.MPFree<int>(p2);
    mp.Display();
    mp.MPFree<short>(p3);
    mp.Display();
	int* p5 = (int*)mp.MPMalloc(sizeof(int));
    mp.Display();
    mp.MPFree<you>(p4);
    mp.Display();
	return 0;
}
