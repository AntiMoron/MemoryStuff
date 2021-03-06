#ifndef __MP_HPP__
#define __MP_HPP__

/**********************
@Author : AntiMoron
@Date   : 2014-8-28
@Topic  : MemoryPool
***********************/

#include<vector>
#include<memory>
#include<memory.h>
#include<cstdio>
#include<type_traits>
#include"BasicStuff.hpp"

class MemoryPool
{
public:
	MemoryPool()
	{
		data = nullptr;
		backwardDist = nullptr;
		poolSize = 1024*1024;
		data = new MSF::BYTE[poolSize];
		//adapt chunk size
		adaptChunkSize();
		//initialize array that recort the distance of next used memory.
		backwardDist = new unit[poolSize];
		backwardDistCount = 1;
		backwardDist[0].dist = poolSize;
		backwardDist[0].position = 0;
	}
	//initialize a memory pool with specified size
	MemoryPool(std::size_t size)
	{
		data = nullptr;
		backwardDist = nullptr;
		poolSize = -1;
		if(size < 0)
		{
			throw std::bad_alloc();
		}
		data = new MSF::BYTE[size];
		if(data == nullptr)
		{
			throw std::bad_alloc();
		}
		else
		{
			poolSize = size;
			//adapt chunk size
			adaptChunkSize();
			//initialize array that recort the distance of next used memory.
			backwardDist = new unit[poolSize];
			backwardDistCount = 1;
			backwardDist[0].dist = poolSize;
			backwardDist[0].position = 0;
		}
	}

	~MemoryPool()
	{
		if(data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
		if(backwardDist != nullptr)
		{
			delete [] backwardDist;
			backwardDist = nullptr;
		}
	}
	template<typename type,int count = 1>
	type* MPAlloc()
	{
		//the result we need.
		type* result = nullptr;
		//check if the pool is full-filled.
		if(backwardDistCount > poolSize)
		{
			return nullptr;
		}
		//Search for the first place of avilable memory
		//and give the place.
		for(int i=0;i < backwardDistCount;i++)
		{
			if(backwardDist[i].dist >= (sizeof(type) * count))
			{
				//if the distance is larger than we need : make an insert operation
				//otherwise just alloc the memory.
				if(backwardDist[i].dist > (sizeof(type) * count))
				{
					++backwardDistCount;
					for(int j=backwardDistCount - 1;j >= (i + 1);j--)
					{
						backwardDist[j].dist = backwardDist[j-1].dist;
						backwardDist[j].position = backwardDist[j-1].position;
					}
					backwardDist[i+1].dist = backwardDist[i].dist - (sizeof(type) * count);
					backwardDist[i+1].position = backwardDist[i].position + (sizeof(type) * count);
					backwardDist[i].dist = 0;
				}
				else
				{
					backwardDist[i].dist = 0;
				}
				result = (type*)(data + backwardDist[i].position);
				if(std::is_constructible<type>::value)
				{
					new(result) type;
				}

				return result;
			}
		}
		//Failed return nullptr.
		return nullptr;
	}


	template<typename type,std::size_t count = 1>
	void MPFree(type* ptr)
	{
		//relative position of base
		std::size_t position = (MSF::BYTE*)ptr - data;
		if(position >= poolSize || (position < 0))
		{
			throw std::exception();
			return ;
		}
		//find whether the ptr is in the array ok to delete.
		MSF::LONG ptrIndex = -1;
		for(int i = 0;i < backwardDistCount;i++)
		{
			if(backwardDist[i].position == position && backwardDist[i].dist == 0)
			{
				ptrIndex = i;
				break;
			}
		}
		if(ptrIndex == -1)
		{
			throw std::exception();
			return ;
		}
		else
		{
			//if it is destructible call the destruct function
			if(std::is_destructible<type>::value)
			{
				ptr->~type();
			}
			//delete the node
			std::size_t size2Free = sizeof(type) * count;
			if(size2Free <=(backwardDist[ptrIndex + 1].position - backwardDist[ptrIndex].position))
			{
				backwardDist[ptrIndex].dist += sizeof(type) * count;
				backwardDist[ptrIndex].position = position;
				//check whether it has previous node.
				//If the previous node is not used.Then merge two nodes.
				if(ptrIndex - 1 >= 0)
				{
					if(backwardDist[ptrIndex - 1].dist > 0)
					{
						backwardDist[ptrIndex - 1].dist += backwardDist[ptrIndex].dist;
						for(int i=ptrIndex; i < backwardDistCount - 1;i++)
						{
							backwardDist[i] = backwardDist[i + 1];
						}
						--backwardDistCount;
						--ptrIndex;
					}
				}
				//check whether it has following node.
				//If the following node is not used.Then merge two nodes.
				if(ptrIndex >= 0 && ptrIndex + 1 < backwardDistCount)
				{
					if(backwardDist[ptrIndex + 1].dist > 0)
					{
						backwardDist[ptrIndex].dist += backwardDist[ptrIndex + 1].dist;
						for(int i = ptrIndex + 1;i < backwardDistCount - 1;i++)
						{
							backwardDist[i] = backwardDist[i + 1];
						}
						--backwardDistCount;
					}
				}
			}
			else
			{
				throw std::exception();
				return ;
			}
		}
	}

	void Display()
	{
		for(int i=0;i<backwardDistCount;i++)
		{
			printf("[%d] : <distance : %d  position : %d> \r\n"
					,i,
					backwardDist[i].dist,
					backwardDist[i].position);
		}
		printf("Totally : %d\r\n\r\n",backwardDistCount);
	}
private:
	void adaptChunkSize()
	{
		chunkSize = poolSize;
		while(chunkSize > 256)
		{
			if(chunkSize % 2 == 0)
				chunkSize /= 2;
		}
	}

	struct unit
	{
		std::size_t dist;
		std::size_t position;
	};

	std::size_t 	chunkSize;		//reserved.
	std::size_t 	poolSize;
	std::size_t		backwardDistCount;
	unit* 			backwardDist;
	MSF::BYTE* 		data;
};

#endif
