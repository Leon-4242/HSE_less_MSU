#include "SALARYBASE.hpp"

namespace SALARYBASE {
        PairTree<std::string, size_t>& SalaryBase::operator[] (const std::string& name) {
            PairTree<std::string, size_t> tmp(name, MROT);
            if (DB.find(tmp)) return DB[name];
            DB.insert(tmp);
            return DB[name];
        }
}

