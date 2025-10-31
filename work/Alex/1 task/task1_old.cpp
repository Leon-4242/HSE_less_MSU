#include <iostream>
#include <vector>

using namespace std;

class ABLTree {
	public:
	class node {
		public:

		int key;
		vector<int> val;
		int diff; 
		int h;
	
		node* left; 
		node* right;
		node* parent;

		static node null;

		node(const int& key, const vector<int>& val): key(key), val(val), diff(0), h(0), left(&null), right(&null), parent(&null) {}

		~node() {
			left = nullptr; right = nullptr;
			parent = nullptr;
			val.clear();
		}
	};

	node* root;
	size_t num;

	void Del(node* curr) {
		if (curr == &node::null) return;
		Del(curr->left); Del(curr->right);
		delete curr;
		return;	
	}

	ABLTree(void): root(&node::null),  num(0) {}
	~ABLTree(void) {
		Del(root);
		num = 0;
	}


	node* newNode (const int& key, const vector<int>& val) {
        ++num;
        node* res = new node(key, val);
        return res;
    }
/*
	void link (const node* parent, const node* child) {
		parent->childs.push_back(child);
		if (parent->subTreeSize == 0) {
			parent->subTreeSize = 1;
			node* curr = parent->parent;
			while (curr != &node) {
				++curr->subTreeSize;
				curr = curr->parent;
			}
		}
		child->parent = parent;
	}

	void add(const int& n_parent)
*/	
	void rotateL(node* a) {
		node* b = a->right;
		a->right = b->left;
		b->left = a;
		
		if (a->parent->left == a) a->parent->left = b;
		else if (a->parent->right == a) a->parent->right = b;
		else root = b;

		b->parent = a->parent;
		a->parent = b;
		a->h = ((a->left->h > a->right->h) ? a->left->h : a->right->h) + 1;
		a->diff = a->left->h - a->right->h;
		b->h = ((a->h > b->right->h) ? a->h : b->right->h) + 1;
		b->diff = a->h - b->right->h;
	}

	void rotateR(node* a) {
		node* b = a->left;
		a->left = b->right;
		b->right = a;
		
		if (a->parent->left == a) a->parent->left = b;
		else if (a->parent->right == a) a->parent->right = b;
		else root = b;

		b->parent = a->parent;
		a->parent = b;

		a->h = ((a->left->h > a->right->h) ? a->left->h : a->right->h) + 1;
		a->diff = a->left->h - a->right->h;
		b->h = ((b->left->h > a->h) ? b->left->h : a->h) + 1;
		b->diff = b->left->h - a->h;
	}

	void rotateBL(node* a) {
		rotateR(a->right);
		rotateL(a);
	}

	void rotateBR(node* a) {
		rotateL(a->left);
		rotateR(a);
	}

	int count(const int& k) {
		node* curr = root;
		while (curr != &node::null) {
			if (k == curr->key) return 1;
			curr = (k < curr->key) ? curr->left : curr->right;
		}
		return 0;
	}

	vector<int>& find(const int& k) {
		node* curr = root;
		while(curr != &node::null) {
			if (k == curr->key) return curr->val;
			curr = (k < curr->key) ? curr->left : curr->right;
		}
		return node::null.val;
	}

	int add_rec(const int& k, node* curr) {
		if (root == &node::null) {
			root = newNode(k, vector<int>());
			root->h = 1;
			root->diff = 0;
			return 0;
		}

		if (k < curr->key) {
			if (curr->left != &node::null) {
				if (!add_rec(k, curr->left)) return 0;
			} else {
				curr->left = newNode(k, vector<int>());
				curr->left->h = 1;
				curr->left->parent = curr;
			}
		} else {
			if (curr->right != &node::null) {
				if (!add_rec(k, curr->right)) return 0;
			} else {
				curr->right = newNode(k, vector<int>());
				curr->right->h = 1;
				curr->right->parent = curr;			
			}
		}
			
		int old_h = curr->h;

		curr->h = ((curr->left->h > curr->right->h) ? curr->left->h : curr->right->h) + 1;
		curr->diff = curr->left->h - curr->right->h;

		if (curr->diff == 2) {
			if (curr->left->diff != -1) rotateR(curr);
			else rotateBR(curr);
			curr = curr->parent;
		} else if (curr->diff == -2) {
			if (curr->right->diff != 1) rotateL(curr);
			else rotateBL(curr);
			curr = curr->parent;
		}

		if (curr->h == old_h) return 0;
		return 1;
	}

	vector<int>& operator[](const int& k) {
		if (!count(k)) add_rec(k, root);
			
		return find(k);
	}

	bool check_rec(const node* curr) {
		if (curr == &node::null) return true;
		if (!(check_rec(curr->left) && check_rec(curr->right))) return false;
		if (curr->left != &node::null && curr->left->key >= curr->key) {cout << "Wrong order" <<endl; return false;}
		if (curr->right != &node::null && curr->right->key <= curr->key) {cout << "Wrong order" << endl; return false;}

		if (curr->h != (curr->left->h > curr->right->h ? curr->left->h : curr->right->h) + 1) {cout << "Wrong height "<< curr->h << endl; return false;}
		if (curr->diff != curr->left->h - curr->right->h) {cout << "Wrong diff" << endl; return false;}

		if (!(curr->diff == 0 || curr->diff == 1 || curr->diff == -1)) {cout << "Not ABL" << endl; return false;}

		return true;
	}

	bool check(void) {
		return check_rec(root);
	}

};


ABLTree::node ABLTree::node::null(0, vector<int>());


void count (const int& k, ABLTree& rev_array, vector<int>& res);

int main(void) {
	int n, buff;
	vector<int> array;
	ABLTree rev_array;
	cin >> n;
	for (int i = 0; i < n-1; ++i) {
		cin >> buff;
		array.push_back(buff);
	}

	for (int i = 0; i < n-1; ++i) rev_array[array[i]] = vector<int>();
	
	for (int i = 0; i < n-1; ++i) {
		rev_array[array[i]].push_back(i+2);
	}

	vector<int> res(n, 0);

	count(1, rev_array, res);

	for (int i = 0; i < n; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
	
	return 0;
}

void count (const int& k, ABLTree& rev_array, vector<int>& res) {
	if (!rev_array.count(k)) {
		res[k-1] = 0;
		return;
	}

	int sum = 0; auto vect = rev_array[k];
	for (auto it = vect.begin(); it != vect.end(); ++it) {
		count(*it, rev_array, res); sum += res[*it -1] + 1;
	}

	res[k-1] = sum;
}
