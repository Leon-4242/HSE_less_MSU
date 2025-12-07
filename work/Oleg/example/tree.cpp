#include <iostream>

class Tree {
	using node = struct Node{
		std::string data;
		struct Node* left; 
		struct Node* right;
	};

	node* root;

	node* newNode(const std::string& value) {
		node* tmp = new node;
		tmp->data = value;
		tmp->left = tmp->right = nullptr;
		return tmp;
	}

	void delNode(node* curr) {
		delete curr;
	}

	void clear(node* curr) {
		if (curr == nullptr) return;
		
		clear(curr->left);
		clear(curr->right);
		delNode(curr);
	}

	void add(node* curr, const std::string& value) {
		if (value < curr->data) {
			if (curr->left  == nullptr) curr->left = newNode(value);
			else add(curr->left, value);
		} else if (value > curr->data) {
			if (curr->right == nullptr) curr->right = newNode(value);
			else add(curr->right, value);
		}
	}

	void print(node* curr) {
		if (curr == nullptr) return;
		print(curr->left);
		std::cout << curr << " " << curr->data << " " << curr->left << " " << curr-> right << std::endl;
		print(curr->right);
	}

	public:
	
	Tree(): root(nullptr) {}
	~Tree() {
		this->clear(root);
	}

	void Clear() {
		clear(root);
	}

	void Add(const std::string& value) {
		if (root == nullptr) root = newNode(value);
		else add(root, value);
	}

	void Print() {
		print(root);
	}
};

int main() {
	Tree tree{};

	for (int  i = 0; i < 5; ++i) {
		tree.Add(std::to_string(i));
	}
	tree.Print();

	return 0;
}
