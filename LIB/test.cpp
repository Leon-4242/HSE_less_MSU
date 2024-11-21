#include <iostream>
#include <functional>
#include "R2Geometry.h"

class Num {
    public: 
    int n;
    Num(int nn): n(nn) {}
    Num(int nn, int mm): n(nn+mm) {}
    void print(int t) const {
        std::cout << "\n"<< n+t << std::endl;
    }
    ~Num() {}
};

int main(void) {
    std::function<void(const Num&, int)> func = &Num::print;
    func(2, 15);
//  std::placeholders::_1
//  std::bind
    return 0;
}
