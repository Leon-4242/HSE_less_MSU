#ifndef SALARYbase
#define SALARYbase

#include "RBTREE.hpp"
#define MROT 19242

namespace SALARYBASE {
    using namespace RBTREE;

    class SalaryBase {
        class NameSalary {
            public:
            std::string name;
            size_t salary;
            bool flag;

            NameSalary(const size_t& val, const std::string = "") {
                name = Name;
                salary = val;
            }

            NameSalary(const Salary& s) {
                name = s.name;
                salary = s.salary;
                flag = s.flag;
            }

            ~NameSalary(void) {}

            NameSalary& operator= (const NameSalary& s) {
                if (s.flag == false) {

                }
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

