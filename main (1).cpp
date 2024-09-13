#include "task1.h"
#include <iostream>
using namespace std;
int main(void){
    CIntN num1(100), num2(10);
    cout << num1.print() << "+" << num2.print() << "=" << (num1 + num2).print() << endl;
    return 0; 
}