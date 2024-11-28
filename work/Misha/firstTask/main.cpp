#include "SALARYBASE.h"

using namespace SALARYBASE;

int main(void) {
   SalaryBase base;
    base["first"] = 20000;
    SalaryBase base1(base), base2;
    base["second"] = 1000;
    SalaryBase base3(base + base2);
    base1 += base2;
    if (base1 == base3) {
        base.remove("first");
    }
        
    std::cout <<base << std::endl;
    std::cout <<base << base1 << base2 <<std::endl;
    base.print(std::cout, "second", "zzzzzzz") << std::endl;
    return 0;
}
