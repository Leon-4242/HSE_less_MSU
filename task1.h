#ifndef TASK1
#define TASK1

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#define N 100


class CIntN
{
    private:
        char arr[N] {};
    public:
        CIntN();
        CIntN(char*);
        CIntN(const CIntN &);
        CIntN (int);
	~CIntN();
	CIntN operator=(const CIntN &);
        CIntN operator+(const CIntN &);
        CIntN operator-(const CIntN &);
        char* print () const;

	CIntN get(int);
};

#endif
