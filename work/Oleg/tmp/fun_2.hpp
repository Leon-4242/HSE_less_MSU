#pragma once 

#include <iostream> 
#include <cstdlib>
#include <cstring>
#include <cassert> 

class CTree
{
  using node = struct Node{struct Node* left; struct Node* right; struct Node* prev; std::string data;};

  node *root;

  node *NewNode(const std::string &a)
  {
    node *tmp = new node;
    tmp->data = a;
    tmp->left = tmp->right = tmp->prev = nullptr;
    return tmp;
  }

  void DelNode(node *curr)
  {
    delete curr;//надо ли освобождать строку? 
  }

  void Add(node *curr, const std::string &a)
  {
    if(a<curr->data)
    {
      if(curr->left)
        Add(curr->left, a);
      else
      {
        node *tmp = NewNode(a);
        tmp->prev = curr;
        curr->left = tmp;
      }
    }
    else if(a>curr->data)
    {
      if(curr->right)
        Add(curr->right, a);
      else
      {
        node *tmp = NewNode(a);
        tmp->prev = curr;
        curr->right = tmp;
      }
    }
    /*else 
      return ;*/
  }

  void ClearBrunch(node *curr)
  {
    if(curr)
    {
      if(curr->left)//сначала подумал, зачем добавил, потом подумал, что меньше раз вызову фукнцию
        ClearBrunch(curr->left);
      if(curr->right)
        ClearBrunch(curr->right);
      /*if (!curr->prev)
				root = nullptr;
        return;*/
      DelNode(curr);
    }
  }

  void print(node* curr)
  {
		if (curr == nullptr) return;
		print(curr->left);
		std::cout << curr << " " << curr->data << " " << curr->left << " " << curr-> right << std::endl;
		print(curr->right);
	}

  node *FindNode(node* curr, const std::string &a)
  {
    if(curr == nullptr)
      return nullptr;
    if(curr->data > a)
      if(curr->left)
        return FindNode(curr->left, a);
      else
        return nullptr;
    else if(curr->data < a)
      if(curr->right)
        return FindNode((curr->right), a);
      else
        return nullptr;
    else
      return curr;
  }

  void del(node* curr, const std::string& a) 
  {
	if (!curr) 
    return;

	if (a < curr->data) 
		del(curr->left, a);
	else if (curr->data < a) 
		del(curr->right, a);
	else
	{
		if (curr->left && curr->right) 
		{
			node* tmp = curr->right;
			while (tmp->left) tmp = tmp->left;

			curr->data = tmp->data;
			del(tmp, tmp->data);
		}
		else if (curr->left)
		{
			if (!curr->prev)
			{
				root = curr->left;
				curr->left->prev = nullptr;
			}
			else 
			{
				curr->left->prev = curr->prev;
				(curr->prev->left == curr) ? (curr->prev->left = curr->left) : (curr->prev->right = curr->left);
			}
		}
		else if (curr->right)
		{
			if (!curr->prev)
			{
				root = curr->right;
				curr->right->prev = nullptr;
			}
			else 
			{
				curr->right->prev = curr->prev;
				(curr->prev->right == curr) ? (curr->prev->right = curr->right) : (curr->prev->left = curr->right);
			}
		}
		else 
		{
			if (!curr->prev)
				root = nullptr;
			else
				(curr->prev->left == curr) ? (curr->prev->left = nullptr) : (curr->prev->right = nullptr);
		}
		DelNode(curr);
	}
  }


  public:

  class iterator
  {
    friend class CTree;

    node* curr;

    node* get(void) {return curr;}

    explicit iterator(node* ptr): curr(ptr) {}

    public: 

    std::string& operator*(void) {return curr->data;}

    iterator& operator++(void)
    {
      assert(curr != nullptr);
      if(curr->right)
      {
        curr = curr->right;
        while(curr->left)
          curr = curr->left;
        return *this;
      }
      while((curr->prev) && curr!=(curr->prev)->left)//тут нужна проверка
        curr = curr->prev;
      if(curr->prev)//по идее эта проверка нужна(если в конечном итоге наткнусь на рут)
        curr = curr->prev;
      return *this;
    }

    iterator& operator--(void)
    {
      assert(curr != nullptr);
      if(curr->left)
      {
        curr = curr->left;
        while(curr->right)
          curr = curr->right;
        return *this;
      }
      while((curr->prev) && curr!=(curr->prev)->right)//тут нужна проверка
        curr = curr->prev;
      if(curr->prev)//по идее эта проверка нужна(если в конечном итоге наткнусь на рут)
        curr = curr->prev;
      return *this;
    }

    bool operator== (const iterator& it) const {return  (it.curr == curr);}
    bool operator!= (const iterator& it) const {return !(it.curr == curr);}
  };

  CTree() : root(nullptr) {};
  ~CTree()
    {this->ClearBrunch(root);};
  
  void Clear() {ClearBrunch(root); root = nullptr;}

  void Addd(const std::string& a)
  {
    if(root)
      Add(root, a);
    else
      root = NewNode(a);
  }

  void Del(const std::string& a)
  {
    del(root, a);
  }

  iterator begin(void)
  {
    if(!root) return iterator(nullptr);
    node* curr = root;
    while(curr->left)
      curr=curr->left;
    return iterator(curr);
  }
  iterator end(void)
  {
    if(!root) return iterator(nullptr);
    node* curr = root;
    while(curr->right)
      curr=curr->right;
    return iterator(curr);
  }


  void PrintTree(void)//вывести все (как массив)
  {
    for(auto it = this->begin(); it!=this->end(); ++it)
      std::cout << *it << std::endl;
    std::cout << *(this->end()) << std::endl;
    return ;
  }

  size_t Size(void)//можно заменить int на size_t для крутизны 
  {
    size_t count = 0;
    for(auto it = this->begin(); it!=this->end(); ++it)
      ++count;
    ++count;
    return count;
  }

  void Print() 
  {
		print(root);
	}

  iterator FindString(const std::string& a) //const //нужно ли тут еще const? видимо нет:)
  {
    node* b = FindNode(root, a);

    return iterator(b);
  }

};
