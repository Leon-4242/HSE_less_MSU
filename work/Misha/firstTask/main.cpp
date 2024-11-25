#include "SALARYBASE.hpp"

using namespace SALARYBASE;

int main(void) {
    try {
        SalaryBase base;
        base["first"] = 20000;
        SalaryBase base1(base), base2;
        base2["second"] = 25000;
        SalaryBase base3(base + base2);
        base1 += base3;
        if (base1 == base3) {
//            base.remove("first");
        }
            std::cout <<base3 << std::endl;
//        std::cout <<base << base1 << base2 <<std::endl;
//        base.print(std::cout, "second", "zzzzzzz") << std::endl;
    } 
    catch( const Except& m) {
        std::cout <<m.what() << std::endl;
    }
    return 0;
}
