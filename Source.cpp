//MemoryPool Test.

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include<list>
#include "BasicStuff.hpp"
#include "mp.hpp"

using namespace std;

HPEN hpen = CreatePen(PS_SOLID,2,RGB(0xff,0xff,0xff));
HPEN bpen = CreatePen(PS_SOLID,2,RGB(0x0,0x0,0x0));
HBRUSH wbrush = CreateSolidBrush(RGB(0xff,0xff,0xff));
HBRUSH gbrush = CreateSolidBrush(RGB(0x0,0xcf,0x0));
HBRUSH bbrush = CreateSolidBrush(RGB(0x0,0x0,0x0));
HBRUSH rbrush = CreateSolidBrush(RGB(0xcf,0x0,0x0));
HBRUSH ybrush = CreateSolidBrush(RGB(0xcf,0xcf,0x0));

MemoryPool mp;
HWND hwnd;
HDC hdc;
RECT g_rect;
HDC buffer;
HDC g_bitmap;
HBITMAP bmp;

int ClearScreen(HDC _hdc)
{
	SelectObject(_hdc,bmp);
	RECT rect;
	rect.left = 0;
	rect.right = 1366;
	rect.top = 0;
	rect.bottom = 768;
	FillRect(_hdc,&rect,bbrush);
	return 0;
}
void randInit()
{
	srand(time(nullptr));
}

long random(long range)
{
	std::size_t randNum = rand() | (rand() << 8) | (rand() << 16) | (rand() << 24);
	return randNum % range;
}

bool randombool()
{
	if(rand() % 2)
	{
		return false;
	}
	return true;
}

float randomf(float range)
{
	float result;
	std::size_t randNum = rand() | (rand() << 8) | (rand() << 16) | (rand() << 24);
	std::size_t irange = range * 10000;
	result = randNum % irange;
	result /= 10000.0f;
	return result;
}

class Pixel
{
public:
    Pixel()
    {

    }
    ~Pixel()
    {
    }

	void init()
	{
		x = 300;
		y = 300;
		r = random(360);
		dr = 0.1f + randomf(3.0f);
		dx = sin(r / 3.1415926);
		dy = -(0.3 + randomf(4.7f));
	}
	void draw()
	{
		x += dx;
		y += dy;
		RECT rect = {x-1,y-1,x+1,y+1};
		FillRect(buffer,&rect,WHITE_BRUSH);
    	r+=dr;
		dx = (0.1f + randomf(10.0f)) * sin(r / 3.1415926);
	}
	bool isOver()
	{
		return y < 0;
	}
private:
	float r,dr;
	float x,y;
	float dx,dy;
};

int main()
{
	SetConsoleTitle("Test");
	while((hwnd = FindWindow(NULL,"Test"))==NULL);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd,&g_rect);
	buffer = CreateCompatibleDC(hdc);
	g_bitmap = CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc,g_rect.right,g_rect.bottom);;
	int pixCount = 0;
	MSF::ULONG anim = 4000;
	std::list<Pixel*>p;
	while(anim--)
	{
		ClearScreen(buffer);

		if(pixCount <= 1000)
		{
			Pixel* pt = mp.MPAlloc<Pixel>();
			pixCount ++;
			pt->init();
			p.insert(p.end(),pt);
		}
		for(std::list<Pixel*>::iterator it = p.begin() ;
			it != p.end() ;
			it++)
		{
			if((*it)->isOver())
			{
				mp.MPFree<Pixel>(*it);
				it = p.erase(it);
				--pixCount;
			}
			else
			{
				(*it)->draw();
			}
		}
		BitBlt(hdc,0,0,g_rect.right - g_rect.left,g_rect.bottom - g_rect.top,buffer,0,0,SRCCOPY);
		Sleep(16);
	}
	return 0;
}
