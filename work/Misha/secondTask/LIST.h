#ifndef LISTOFALL
#define LISTOFALL

#include <iostream>
#include "EXCEPT.h"

namespace LIST {
    using namespace EXCEPT;
    template <typename T>
    class List;

    template <typename T>
    std::ostream& operator<< (std::ostream&, const List<T>&);
    template <typename T>
    std::istream& operator>> (std::istream&, List<T>&);

    template <typename T>
    class List {
        template <typename V>
        class Node {
            public:

            V mes;
            Node* last;
            Node* next;

            Node(const V text): mes(text) {
                next = nullptr;
                last = nullptr; 
            }

            ~Node() {
                last = nullptr;
                next = nullptr;
            }

            V val(void) const {
               return mes;
            }
        };

        template <typename V>
        class iterator {
            public:

            const List<V> *lst;
            Node<V>* pos;
            size_t index;

            iterator (const List<V>* list = nullptr, Node<V>* node = nullptr, const size_t ind = 0): index(ind) {
                lst = list;
                pos = node;
            }

            ~iterator(void) {
                lst = nullptr;
                pos = nullptr;
            }

            iterator& operator++(void) {
                if (pos == nullptr) return *this;
                pos = pos->next;
                index++;
                return *this;
            }
            iterator operator++(int) {
                if (pos == nullptr) return *this;
                iterator old = *this;
                ++(*this);
                return old;
            }

            iterator& operator--(void) {
                if (pos == nullptr) return *this;
                pos = pos->last;
                index--;
                return *this;
            }

            iterator operator--(int) {
                if (pos == nullptr) return *this;
                iterator old = *this;
                --(*this);
                return old;
            }

            iterator operator+(size_t k) {
                if (pos == nullptr) return *this;
                auto tmp = pos;
                for (size_t i = 0; i < k; ++i) {
                    if (tmp != nullptr) tmp = tmp->next;
                    else return lst->end();
                }
                return iterator(lst, tmp, index+k);
            }

            bool operator== (const iterator& i) const {
                return (lst == i.lst && index == i.index) ? true : false;
            }

            bool operator!= (const iterator& i) const {
                return !(*this == i);
            }

            V& operator* (void) {
                return pos->mes;
            }

            size_t Index (void) {
                return index;
            }
        };

        Node<T>* head;
        Node<T>* back;
        size_t size;

        public:

        List(void){
            head = nullptr;
            back = nullptr;
            size = 0;
        }

        ~List() {
            while (size != 0) {
                if (size == 1) {
                    delete back;
                    back = nullptr; head = nullptr;
                    size = 0;
                    continue;
                }
                back = back->last;
                back->next->last = nullptr;
                delete back->next;
                back->next = nullptr;
                size--;
            }
        }

        List(const List& ls) {
            size = 0;
            for (auto iter = ls.begin(); iter != ls.end(); ++iter) {
                if (size == 0) {
                    back = new Node<T>(*iter);
                    head = back;
                    size++;
                    continue;
                }
                back->next = new Node<T>(*iter);
                back->next->last = back;
                back = back->next;
                size++;
            }
        }

        List(List&& move) {
       		size = move.size;
        	move.size = 0;
        	head = move.head;
        	move.head = nullptr;
        	back = move.back;
        	move.back = nullptr;
    	}
    	
        List& operator= (const List& ls) {
        	this->clear();
        	if (ls.empty()) return *this;
        	for (auto iter = ls.begin(); iter != ls.end(); ++iter) {
        	    this->pushBack(*iter);
        	}

        	return *this;
    	}
    	
        List& operator= (List&& move) {
    	    this->clear();

    	    size = move.size;
    	    move.size = 0;
    	    head = move.head;
    	    move.head = nullptr;
    	    back = move.back;
    	    move.back = nullptr;
	
   		    return *this;
    	}

		void pushBack(const T text) {
    	    if (size == 0) {
   		        back = new Node<T>(text);
    	        head = back;
        	    size = 1;
        	    return;
        	}
			back->next = new Node<T>(text);
			back->next->last = back;
			back = back->next;
        	size++;
		}
		
        void popBack(void) {
        	if (size == 0) {
        	    return;
        	}
        	if (size == 1) {
        	    delete back;
        	    back = head = nullptr;
        	} else {
        	    back = back->last;
        	    delete back->next;
        	    back->next = nullptr;
        	}
        	size--;
    	}

        void pushHead(const T text) {
        	if (size == 0) {
        	    head = new Node<T>(text);
        	    back = head;
        	    size = 1;
        	    return;
        	}
        	head->last = new Node<T>(text);
        	head->last->next = head;
        	head = head->last;
        	size++;
    	}
    	
        void popHead(void) {
        	if (size == 0) return;
        	if (size == 1) {
        	    delete back;
        	    back = head = nullptr;
        	} else {
        	    head = head->next;
        	    delete head->last;
        	    head->last = nullptr;
        	}
        	size--;
    	}

        void pushIn(const size_t k, const T text) {
        	if (k > size) throw Except("out of range");
        	if (k == 0) {
        	    this->pushHead(text);
        	    return;
        	}
        	if (k == size) {
        	    this->pushBack(text);
        	    return;
        	}
        	auto iter = this->begin();
        	for (size_t i = 0; i < k; ++i) ++iter;
        	auto block = new Node<T>(text);

        	iter.pos->last->next = block;
        	block->next = iter.pos;
        	block->last = iter.pos->last;
        	iter.pos->last = block;
        	size++;
    	}
		 
        void pushIn (iterator<T> iter, const T text) {
            if (iter.Index() == 0) {
                this->pushHead(text);
                return;
            }
            if (iter.Index() == size) {
                this->pushBack(text);
                return;
            }
            auto block = new Node<T>(text);
            iter.pos->last->next = block;
            block->next = iter.pos;
            block->last = iter.pos->last;
            iter.pos->last = block;
            size++;
        }
        
        void popIn(const size_t k) {
        	if (k > size) throw Except("out of range");
        	if (k == 0) {
        	    this->popHead();
        	    return;
        	}
        	if (k == size) {
        	    this->popBack();
        	    return;
        	}

        	auto iter = this->begin() + k;
        	iter.pos->last->next = iter.pos->next;
        	iter.pos->next->last = iter.pos->last;
        	delete iter.pos;
        	size--;
    	}     

        void popIn(iterator<T> iter) {
            if (iter.Index() == 0) {
                this->popHead();
                return;
            }
            if (iter.Index() == size) {
                this->popBack();
                return;
            }

            iter.pos->last->next = iter.pos->next;
            iter.pos->next->last = iter.pos->last;
            delete iter.pos;
            size--;
        }
        
        bool empty(void) const {
        	return size == 0;
   		}
        size_t length(void) const {
        	return size;
    	}
        
    	List add(const List* curr) {
    	    List res = *this;
    	    for (auto iter = curr->begin(); iter != curr->end(); ++iter) {
    	        res.back->next = new Node<T>(*iter);

    	        res.back->next->last = res.back;
    	        res.back = res.back->next;
    	        res.size++;
    	    }
    	    return res;
    	}
    	
        void clear (void) {
    	    while (size > 0) {
    	        this->popBack();
    	    }
    	}
	    
   		List sort(int (*op)(const T, const T)) {
        	List tmp; bool flag = true;
        	for (auto iter = this->begin(); iter != this->end(); ++iter) {
        	    if (tmp.empty()) {
        	        tmp.pushBack(*iter);
        	        continue;
        	    }
	
    	        for (auto it = tmp.begin(); it != tmp.end(); ++it) {
    	            if (op(*iter, *it) == 1) continue;
    	            else {
    	                tmp.pushIn(it, *iter);
    	                flag = false;
    	                break;
        	        }
        	    }
        	    if (flag) tmp.pushBack(*iter); 
        	    else flag = true;

        	}
        	return tmp;
    	}
        
        void swap (iterator<T> i1, iterator<T> i2) {
    	    List<T> ls;
        
    	    for (auto iter = i1.lst->begin(); iter != i1.lst->end(); ++iter) {
    	        if (iter == i1) {
    	            ls.pushBack(*i2);
    	        }
    	        else if (iter == i2) {
    	            ls.pushBack(*i1);
    	        }
    	        else {
    	            ls.pushBack(*iter);
    	        }
    	    }
    	    *this = ls;
    	}

    		iterator<T> begin(void)  const{
    	    return iterator<T>(this, head, 0);
    	}

    	iterator<T> end(void) const {
    	    return iterator<T>(this, back->next, size);
    	}

		static std::string name (void) {
			return std::string("List");
		}

    };

    template <typename T>
    std::ostream& operator<< (std::ostream& os, const List<T>& list) {
        if (list.empty()) return os << "\n";
        for (auto iter = list.begin(); iter != list.end(); ++iter) {
            os << *iter << "\n";
        }

        return os;
    }

    template <typename T>
    std::istream& operator>> (std::istream& is, List<T>& list) {
        list.clear();
        size_t num = 0;
        is >> num;
        
        for (size_t i = 0; i < num; ++i) {
            T tmp;
            is >> tmp;
            list.pushBack(tmp);
        }

        return is;
    }
}

#endif
