#ifndef SALARYbase
#define SALARYbase

#include "RBTREE.hpp"
#define MROT 19242

namespace SALARYBASE {
    using namespace RBTREE;

    class SalaryBase {
        class Salary {
            public:
            size_t salary;
            bool flag;

            Salary(const size_t& val) {
                flag = (val >= MROT) ? true : false;
                salary = val;
            }

            Salary(const Salary& s) {
                salary = s.salary;
                flag = s.flag;
            }

            ~Salary(void) {}

            Salary& operator= (const Salary& s) {
                salary = s.salary; flag = s.flag;
                return *this;
            }

        }
        RBTree<std::string, size_t> data;
        
        public:
        SalaryBase(void): data() {}
        SalaryBase(const SalaryBase& base): data(base.data) {}
        SalaryBase& operator= (const SalaryBase& base) {
            data = base.data;
            return *this;
        }
        ~SalaryBase(void) {}
        
        size_t& operator[] (const std::string& name) {
            size_t tmp = MROT;
            if (data.find(name, tmp)) return data[name];
            data.insert(name, tmp);

            return data[name];
        }

        friend std::ostream& operator<< (std::ostream& os, const SalaryBase& base) {
            return os << base.data;
        }
    };
}

#endif

