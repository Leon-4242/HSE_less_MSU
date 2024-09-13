#include "task1.h"
#include <cstring>

using namespace std;
CIntN::CIntN() {
    cout << "Default constructor for " << this->print() << endl;
}
CIntN::CIntN(char* s){
    int len_s = strlen(s);
    for(int i = 0; i < N; i++){
        if('0' > s[len_s - 1 - i] || s[len_s - 1 - i] > '9'){
            cout << "wrong number" << endl;
            break;
        }
        arr[N-1-i] = s[len_s - 1 - i] - '0';
    }
}
CIntN::CIntN(const CIntN &a){
    for(int i = 0; i < N; i++){
        arr[i] = a.arr[i];
    }
}
CIntN::CIntN(int num){
    int index = N-1;
    while (index >= 0){
        arr[index] = num % 10;
        num = num/10;
        index--;
    }
}
CIntN CIntN::operator+(const CIntN &a){
    int elem, perenos = 0;
    char* array;
    array = new char [N];
    for(int i = N-1; i >= 0; i--){
        elem = arr[i] + a.arr[i] + perenos;
        perenos = elem / 10;
        elem = elem % 10;
        array[i] = elem+48;
    }
    if (perenos) cout << "perepolnenie" << endl;
    CIntN p(array);
    delete[] array;
    return p;
}
CIntN CIntN::operator-(const CIntN &a){
    int elem, perenos = 0;
    char* array;
    array = new char [N];
    for(int i = N-1; i >= 0; i --){
        elem = arr[i] - a.arr[i] - perenos;
        perenos = 0;
        if (elem < 0){ 
            elem += 10; 
            perenos = 1;
        }
        array[i] = elem;
    }
    if (perenos) cout << "perepolnenie" << endl;
    CIntN b(array);
    delete[] array;
    return b;

}
char* CIntN::print() const{
    bool flag = true;
    char* res;
    int k;
    for (int i = 0; i < N; i++){
        if (flag){
            if (arr[i] != 0){
                flag = false;
                k = N - i;
                res = new char [k + 1];
                res[0] = arr[i] + 48;
            }
        }
        else {
            res[i - N + k] = arr[i] + 48;
        }    
    }
    if (flag) {
        res = new char [2];
        res[0] = 48;
        res[1] = '\0';
    }
    else {
        res[k] = '\0';
    }
    return res;
}
