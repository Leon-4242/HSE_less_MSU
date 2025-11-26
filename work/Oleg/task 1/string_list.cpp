#include "string_list.hpp"

CStringList::CStringList() : head(nullptr), tail(nullptr), size(0) {}

CStringList::~CStringList() {
    Clear();
}

void CStringList::AddToEnd(const char* str) {
    if (size == 0) {
		head = new node;
		head->next = nullptr;
		head->prev = nullptr;
		head->data  = new char[strlen(str)+1];
		strcpy(head->data, str);
		size = 1;
		tail = head;
	} else {
		++size;
		tail->next = new node;
		tail->next->prev = tail;
		tail = tail->next;
		tail->next = nullptr;
		tail->data = new char[strlen(str)+1];
		strcpy(tail->data, str);
	}
}

void CStringList::AddAll(const CStringList& other) {
	node* other_curr = other.head;
	node* curr = tail;

	while (other_curr != nullptr) {
		curr->next = new node;
		curr->next->prev = curr;
		curr = curr->next;
		curr->next = nullptr;
		curr->data = new char[strlen(other_curr->data)+1];
		strcpy(curr->data, other_curr->data);
		other_curr = other_curr->next;
	}
	size += other.size;
}

void CStringList::InsertAt(size_t index, const char* str) {
    if (index < 0 || index > size) return;
	++size;

	node* tmp = new node;
	tmp->data = new char[strlen(str)+1];
	strcpy(tmp->data, str);

	if (index == 0) {
		tmp->next = head;
		head->prev = tmp;
		head = tmp;
	} else if (index == size-1){
		tail->next = tmp;
		tmp->prev = tail;
		tail = tail->next;
		tail->next = nullptr;
	} else {
		node* curr = head;
		while (index > 1) {curr = curr->next; --index;}

		tmp->next = curr->next->next;
		curr->next->next->prev = tmp;

		tmp->prev = curr;
		curr->next = tmp;
	}
}

CStringList::iterator CStringList::begin(void) {
    return iterator(head, 0);
}

CStringList::iterator CStringList::end(void) {
    return iterator(tail->next, size);
}

void CStringList::RemoveIt(iterator it) {
	node* rem = it.get();
	if (size == 1) {
		head = nullptr;
		tail = nullptr;
	} else if (0 == it.index()) {
		head = head->next;
		head->prev = nullptr;
	} else if (size-1 == it.index()) {
		tail = tail->prev;
		tail->next = nullptr;
	} else if (0 < it.index() && it.index() < size) {
		rem->next->prev = rem->prev;
		rem->prev->next = rem->next;
	}

	--size;
	delete[] rem->data;
	delete rem;
}

void CStringList::RemoveAt(size_t index) {
    if (index < 0 || index >= size) return;
	auto it = this->begin();

	while(index > 0) {++it; --index;}

	RemoveIt(it);
}

size_t CStringList::GetSize() const {
    return size;
}

void CStringList::Clear() {
	if (size == 0) return;

	node* curr = head;

	while (curr && curr->next) {
		curr = curr->next;
		delete[] curr->prev->data;
		delete curr->prev;
	}
	delete[] curr->data;
	delete curr;
	head = nullptr;
	tail = nullptr;
	size = 0;
}

void CStringList::QuickSort(size_t n, node* left, node* right, int (*compare)(const char*, const char*)) {
    if (n == 1 || n == 0) return;
	node* pivot = left;
	for (int i = 0; i < n/2; ++i) pivot = pivot->next;
    node* l = left; node* r = right;
	
	int i = 0, j = n-1;
    while (i <= j) {
		while (compare(l->data, pivot->data) < 0) {++i; l = l->next;}
        while (compare(r->data, pivot->data) > 0) {--j; r = r->prev;}
        if (i >= j) break; 
		
		CStringList::swap(l, r);
		if (r == left) left = l;
		if (l == right) right = r;

		l = l->next;
		r = r->prev;
		++i; --j;

    }
	
	QuickSort(i, left, l->prev, compare);
    QuickSort(n-i, l, right, compare);
}

void CStringList::Sort(int (*compare)(const char*, const char*)) {
    if (size <= 1) return;
    QuickSort(size, head, tail, compare);
}

void CStringList::swap(node*& a, node*& b) {
	if (a == b) return;

	if (b == head) swap(b, a);
	if (a == tail) swap(b, a);
	
	node* curr = a->prev;
	(curr ? curr->next : head) = b;
	a->prev = b->prev;
	b->prev = curr;
	a->prev->next = a;

	curr = b->next;
	(curr ? curr->prev : tail) = a;
	b->next = a->next;
	a->next = curr;
	b->next->prev = b;

	curr = a;
	a = b;
	b = curr;
}
