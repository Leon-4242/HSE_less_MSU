#ifndef TASK1
#define TASK1

#include <iostream>
#define N 100


class CIntN
{
    private:
        unsigned int arr[N] {};
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
