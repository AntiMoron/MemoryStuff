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
HBRUSH bbrush = CreateSolidBrush(RGB(0x0,0x0,0x0));
HBRUSH wbrush = CreateSolidBrush(RGB(0xff,0xff,0xff));
HBRUSH w2brush = CreateSolidBrush(RGB(0xcc,0xcc,0xcc));
HBRUSH w3brush = CreateSolidBrush(RGB(0xaa,0xaa,0xaa));
HBRUSH w4brush = CreateSolidBrush(RGB(0x66,0x66,0x66));
HBRUSH w5brush = CreateSolidBrush(RGB(0x33,0x33,0x33));

MemoryPool mp;
HWND hwnd;
HDC hdc;
RECT g_rect;
HDC buffer;
HDC g_bitmap;
HBITMAP bmp;
const int traceCount = 10;

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
		init();
    }
    ~Pixel()
    {
    }
	void draw()
	{
		x += dx;
		y += dy;
		for(int i=traceCount - 2;i>=0;i--)
		{
			histX[i+1] = histX[i];
			histY[i+1] = histY[i];
		}
		histX[0] = x;
		histY[0] = y;
    	r+=dr;
		dx = (0.1f + randomf(10.0f)) * sin(r / 3.1415926);
		for(int i=0;i<traceCount;i++)
		{
			RECT rect = {histX[i]-1.0f,histY[i]-1.0f,histX[i]+1.0f,histY[i]+1.0f};
			if(i <= (traceCount/5))	FillRect(buffer,&rect,w2brush);
			else if(i <= (2 * traceCount/5))	FillRect(buffer,&rect,w3brush);
			else if(i <= (3 * traceCount/5))	FillRect(buffer,&rect,w4brush);
			else if(i <= (4 * traceCount/5))	FillRect(buffer,&rect,w4brush);
			else if(i <= traceCount)	FillRect(buffer,&rect,w5brush);
		}
		RECT rect = {x-1,y-1,x+1,y+1};
		FillRect(buffer,&rect,WHITE_BRUSH);
	}
	bool isOver()
	{
		return y < 0;
	}
private:
	float r,dr;
	float x,y;
	float dx,dy;
	float histX[traceCount],histY[traceCount];

	void init()
	{
		x = 300;
		y = 300;
		r = random(360);
		dr = 0.1f + randomf(3.0f);
		dx = sin(r / 3.1415926);
		dy = -(0.3 + randomf(4.7f));
		for(int i=0;i<3;i++)
		{
			histX[i] = x;
			histY[i] = y;
		}
	}
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
	MSF::ULONG anim = 10000;
	std::list<Pixel*>p;
	while(anim--)
	{
		ClearScreen(buffer);

		if(pixCount <= 1000)
		{
			try{
				Pixel* pt = mp.MPAlloc<Pixel>();
				pixCount ++;
				p.insert(p.end(),pt);
			}
			catch(const exception& e){}
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
