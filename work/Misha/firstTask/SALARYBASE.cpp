#include "SALARYBASE.h"

namespace SALARYBASE {
        size_t& SalaryBase::operator[] (const std::string& name) {
            size_t tmp(MROT);
            if (DB.find(name, tmp)) return DB[name];
            DB.insert(name, tmp);
            return DB[name];
        }
}
