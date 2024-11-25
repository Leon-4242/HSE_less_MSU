#ifndef SALARYbase
#define SALARYbase

#define MROT 19242
#include "RBTREE.hpp"

namespace SALARYBASE {
    using namespace RBTREE;

    class Base: public RBTree<std::string, size_t> {
        public:
        Base (const RBTree<std::string, size_t>& tree = RBTree<std::string, size_t>()) : RBTree<std::string, size_t>(tree) {}
        friend  std::ostream& operator<< (std::ostream& os, node* n) {
            if (n == &node::null) return os << "\nLIST\n";
            if (n->Value() < MROT) n->flag = false;
            else n->flag = true;
            if (!n->flag) return os;
            return os << "\nKey: " << n->Key() << " Value: " << n->Value() << "\n";
        }
    };

    class SalaryBase {
        Base DB;

        public:

        SalaryBase(const Base db = Base()): DB(db) {}
        SalaryBase(const SalaryBase& database): DB(database.DB) {}
        SalaryBase& operator= (const SalaryBase& database) {DB = database.DB; return *this;}
        ~SalaryBase(void) {}

        size_t& operator[] (const std::string& name);
        friend std::ostream& operator<< (std::ostream& os, const SalaryBase& database) {return os << database.DB;}
        std::ostream& print(std::ostream& os, const std::string& k1, const std::string& k2) {return DB.Print(os, k1, k2);}

        SalaryBase operator+ (const SalaryBase& database) const {return SalaryBase(DB+database.DB);}

        SalaryBase& operator+= (const SalaryBase& database) {return *this = *this + database;}

        bool operator== (const SalaryBase& database) const {return DB == database.DB;}
        bool operator!= (const SalaryBase& database) const {return DB != database.DB;}

        void remove(const std::string& name) {DB.remove(name);}
    };
}

#endif

