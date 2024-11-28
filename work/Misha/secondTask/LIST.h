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
        class Node {
            public:

            T mes;
            Node* last;
            Node* next;
            Node(const T& val = T()): mes(val) {
                next = nullptr;
                last = nullptr;
            }

            ~Node() {
                last = nullptr;
                next = nullptr;
            }

            const T& val(void) const {return mes;}
            T& val(void) {return  mes;}
        };

        class iterator {
            public:

            const List* lst;
            Node* pos;
            size_t index;

            iterator (const List* list = nullptr, Node *node = nullptr, const size_t ind = 1): lst(list), pos(node), index(ind) {}

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

            int operator- (const iterator& i) const {
                return (index-i.index);
            }

            bool operator== (const iterator& i) const {
                return (lst == i.lst && index == i.index) ? true : false;
            }

            bool operator!= (const iterator& i) const {
                return !(*this == i);
            }

            T& operator* (void) {
                return pos->mes;
            }

            size_t Index (void) {
                return index-1;
            }
        };

        class riterator {
            public:

            const List *lst;
            Node* pos;
            int index;

            riterator (const List* list = nullptr, Node* node = nullptr, const size_t ind = 1): lst(list), pos(node), index(ind) {}

            ~riterator(void) {
                lst = nullptr;
                pos = nullptr;
            }

            riterator& operator++(void) {
                if (pos == nullptr) return *this;
                pos = pos->last;
                index--;
                return *this;
            }
            riterator operator++(int) {
                if (pos == nullptr) return *this;
                iterator old = *this;
                ++(*this);
                return old;
            }

            riterator& operator--(void) {
                if (pos == nullptr) return *this;
                pos = pos->next;
                index++;
                return *this;
            }

            riterator operator--(int) {
                if (pos == nullptr) return *this;
                iterator old = *this;
                --(*this);
                return old;
            }

            riterator operator+(size_t k) {
                if (pos == nullptr) return *this;
                auto tmp = pos;
                for (size_t i = 0; i < k; ++i) {
                    if (tmp != nullptr) tmp = tmp->last;
                    else return lst->rbegin();
                }
                return riterator(lst, tmp, index-k);
            }

            bool operator== (const riterator& i) const {
                return (lst == i.lst && index == i.index);
            }

            bool operator!= (const riterator& i) const {
                return !(*this == i);
            }

            T& operator* (void) {
                return pos->mes;
            }

            size_t Index (void) {
                return index-1;
            }
        };

        Node* head;
        Node* back;
        Node* null;
        size_t size;

        public:

        List (void){
            head = nullptr;
            back = nullptr;
            null = new Node();
            null->last = back;
            null->next = head;
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
            delete null;
        }

        List(const List& ls) {
            size = 0;
            for (auto iter = ls.begin(); iter != ls.end(); ++iter) {
                if (size == 0) {
                    back = new Node(*iter);
                    head = back;
                    size++;
                    continue;
                }
                back->next = new Node(*iter);
                back->next->last = back;
                back = back->next;
                size++;
            }
            null = new Node();
            null->next = head; null->last = back;
        }

        List(List&& move) {
       		size = move.size;
        	move.size = 0;
        	head = move.head;
        	move.head = nullptr;
        	back = move.back;
        	move.back = nullptr;
            null = move.null;
            move.null = nullptr;
    	}
    	
        List& operator= (const List& ls) {
        	this->clear();
        	if (ls.empty()) return *this;
        	for (auto iter = ls.begin(); iter != ls.end(); ++iter) {
        	    this->pushBack(*iter);
        	}
            null->next = head; null->last = back;
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
	        null = move.null;
            move.null = nullptr;
   		    return *this;
    	}

		void pushBack(const T text) {
    	    if (size == 0) {
   		        back = new Node(text);
    	        head = back;
        	    size = 1;
        	    return;
        	}
			back->next = new Node(text);
			back->next->last = back;
			back = back->next;
        	size++;
	        null->next = head; null->last = back;
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
	        null->next = head; null->last = back;
    	}

        void pushHead(const T text) {
        	if (size == 0) {
        	    head = new Node(text);
        	    back = head;
        	    size = 1;
        	    return;
        	}
        	head->last = new Node(text);
        	head->last->next = head;
        	head = head->last;
        	size++;
	        null->next = head; null->last = back;
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
	        null->next = head; null->last = back;
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
        	auto block = new Node(text);

        	iter.pos->last->next = block;
        	block->next = iter.pos;
        	block->last = iter.pos->last;
        	iter.pos->last = block;
        	size++;
	        null->next = head; null->last = back;
    	}
		 
        void pushIn (iterator iter, const T text) {
            if (iter.Index() == 0) {
                this->pushHead(text);
                return;
            }
            if (iter.Index() == size) {
                this->pushBack(text);
                return;
            }
            auto block = new Node(text);
            iter.pos->last->next = block;
            block->next = iter.pos;
            block->last = iter.pos->last;
            iter.pos->last = block;
            size++;
	        null->next = head; null->last = back;
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
	        null->next = head; null->last = back;
    	}     

        void popIn(iterator iter) {
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
	        null->next = head; null->last = back;
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
    	        res.back->next = new Node(*iter);

    	        res.back->next->last = res.back;
    	        res.back = res.back->next;
    	        res.size++;
    	    }
	        null->next = head; null->last = back;
    	    return res;
        }
    	
        void clear (void) {
    	    while (size > 0) {
    	        this->popBack();
    	    }
	        null->next = head; null->last = back;
    	}

        iterator begin(void)  const{
            return iterator(this, head, 1);
        }

        iterator end(void) const {
            return iterator(this, null, size+1);
        }

        riterator rbegin(void) const {
            return riterator(this, back, size);
        }

        riterator rend(void) const {
            return riterator(this, null, 0);
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
