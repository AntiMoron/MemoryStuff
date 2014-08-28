#include <windows.h>
#include <iostream>
#include "mp.hpp"
using namespace std;

class CTestClass
{
    char m_chBuf[4096];
};

int main()
{
    DWORD count = GetTickCount();
	MemoryPool mp;
    for(unsigned int i=0; i<0x5fffff; i++)
    {
//        CTestClass *p = new CTestClass;  2231ms
//        delete p;
//		CTestClass* p = (CTestClass*)mp.MPMalloc(sizeof(CTestClass)); 515 ms
//		mp.MPFree<CTestClass>(p);
    }

    cout << "Interval = " << GetTickCount()-count << " ms" << endl;

    return 0;
}
//2231ms
