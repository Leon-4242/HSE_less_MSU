#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

class CStringList {
    using node = struct Node{struct Node* next; struct Node* prev; char* data;};

	node* head;
	node* tail;
    size_t size;

public:
	class iterator {
		friend class CStringList;
		node* curr;
		size_t ind;
		explicit iterator(node* ptr, const size_t i): curr(ptr), ind(i) {}
		
		node* get(void) {return curr;}
		public:

		char*& operator*(void) {return curr->data;}
	
		iterator& operator++(void) {
			assert(curr != nullptr); 
			++ind;
			curr = curr->next;
			return *this;
		}
		iterator& operator--(void){
			assert(curr != nullptr);
			--ind;
			curr = curr->prev;
			return *this;
		}

		size_t index(void) {return ind; }

		bool operator== (const iterator& it) const {
			return (it.curr == curr && it.ind == ind);
		}
		bool operator!= (const iterator& it) const {
			return !(*this == it);
		}
	};

    CStringList();
    ~CStringList();

    void AddToEnd(const char* str);
    void AddAll(const CStringList& other);
    void InsertAt(size_t index, const char* str);

    iterator begin(void);
    iterator end(void);

    void RemoveIt(iterator it);
    void RemoveAt(size_t index);

    size_t GetSize() const;
    void Clear();

    void Sort(int (*compare)(const char*, const char*));

private:
    void QuickSort(size_t, node*, node*, int (*compare)(const char*, const char*));
	void swap(node*&, node*&);
};

