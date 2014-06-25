/***************************
//@love letter
//Author : AntiMoron
//Date   : 2014-05-24
***************************/

#include"BasicStuff.hpp"
#include"AutoArgumentType.hpp"

#include<iostream>

struct you
{
	you(bool _val = false) : LoveMe(_val)
	{
		troubles = nullptr;
	}
	bool LoveMe;
	char* troubles;
};

int main()
{
	you U;
	auto I = make_guard(
						[&]()
						{
							std::cout<<"I'll be your guard whether you love me or not."<<std::endl;
							if(U.troubles)
							{
								delete[] U.troubles;
								U.troubles = nullptr;
							}
						});
	U.troubles = new char[256];
	std::cout<<"Guard your memory.Keep you safe."<<std::endl;
	return 0;
}
