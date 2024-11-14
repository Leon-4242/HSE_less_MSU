#ifndef INFACE
#define INFACE

#include <type_traits>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "EXCEPT.h"

namespace INTERFACE {
	using namespace EXCEPT;
	
	template <typename T>
    class InFace {

        protected:
        T* vars;
        size_t num;

        public:
		
		InFace (void) {
			num = 0;
			vars = nullptr;
		}

		InFace (const T& curr) {
			num = 1;
			vars = new T[num];
			vars[0] = curr;
		}

		~InFace (void) {
			delete[] vars;
		}

		InFace<T>& operator+= (const InFace<T>& inface) {
			num += inface.num;
			auto tmp = new T[num];
			for (size_t i = 0; i < num; ++i) {
			    tmp[i] = (i < num - inface.num) ? vars[i] : inface.vars[i+inface.num-num];
			}

			delete[] vars;
			vars = tmp;
			return *this;
		}
		
		size_t length (void) const {
			return num;
		}
    
		friend std::ostream& operator<< (std::ostream& os, const InFace<T>& inface) {
			for (size_t i = 0; i < inface.length(); ++i) {
				os << inface.vars[i] << "\n";
			}

			return os;
		}

		T& operator[] (const size_t k) {
			if (k >= num) throw Except("out of interface");

			return vars[k];
		}
	
		bool add (void) {
            int way = 0;
            std::cout <<"\nChoose way for input:\n1)Console\n2)File\n3)Generate" << std::endl;
			std::cin >> way;
            if (way != 1 && way != 2 && way != 3) return true;
            T tmp;
            if (way == 1) {
	    		std::cout <<"\nEnter " << T::name() << ":" << std::endl;
			    std::cin >> tmp;
            } else if (way == 2) {
                std::string filename;
                std::cout << "\nEnter filename:" << std::endl;
                std::cin >> filename;
                std::ifstream file; file.open(filename, std::ios::in);
                if (file.is_open()) {
                    file >> tmp;
                } else {
                    return true;
                }
            } else {
                srand((int)time(NULL));
                tmp.generate();
            }

            *this += tmp;
            return false; 
		}
    
		bool print (void) {
            int way = 0;
            std::cout <<"\nChoose way for data:\n1)Console\n2)File" << std::endl;
            std::cin >> way;
            if (way != 1 && way != 2) return true;
            if (way == 1) {
        	    for (size_t i = 0; i < num; ++i) {
			        std::cout << "\n" << T::name() << " number " << i << ":\n" << vars[i] << std::endl;
			    }
            } else {
                std::string filename;
                std::cout << "\nEnter filename:" << std::endl;
                std::cin >> filename;
                std::ofstream file; file.open(filename, std::ios::out);
                if (file.is_open()) {
        	        for (size_t i = 0; i < num; ++i) {
			            file << "\n" << T::name() << " number " << i << ":\n" << vars[i] << "\n";
			        }
                } else {
                    return true;
                }
            }

            return false;
		}

		virtual bool action (void) {
			std::cout <<"Not ready yet." << std::endl;
			return false;
		} 

		void start(void) {
		    int act = 0;
		    while (true) {
		        std::cout << "\nChoose action:\n1)Add list\n2)Print\n3)Make action" << std::endl;
		        std::cin >> act;
		        if (act != 1 && act != 2 && act != 3) {
		            break;
		        }
	            if (act == 1) {
	                
	                if(this->add()) break;
	
	            } else if (act == 2) {
            
	                if (this->print()) break;
	            
	            } else {
	                if (this->action()) break;
	            }
	        }
		}
    };
}

#endif
