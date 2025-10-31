#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int max(int, int);
int min(int, int);

class Tree {
private:
    class Node {
    public:
        int key;
        Node* left;
        Node* right;
		Node* parent;

        Node(int k)
            : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
        ~Node() {
            left = nullptr;
            right = nullptr;
			parent = nullptr;
        }
    };

    Node* root;

    // Повороты
    Node* rotateRight(Node* y) {
        Node* x = y->left;

		y->left = x->right;
		if (y->left != nullptr) {y->left->parent = y;}

        x->right = y;
		if (y->parent != nullptr) {
			if (y->parent->left == y) y->parent->left = x;
			if (y->parent->right == y) y->parent->right = x;
		} else root = x;

		x->parent = y->parent;
		y->parent = x;
		
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;

		x->right = y->left;
		if (x->right != nullptr) {x->right->parent = x;}

        y->left = x;
        if (x->parent != nullptr) {
			if (x->parent->left == x) x->parent->left = y;
			if (x->parent->right == x) x->parent->right =y;
		} else root = y;

		y->parent = x->parent;
		x->parent = y;

        return y;
    }

    // Вставка
    void insert(Node* node, int key) {
		if (root == nullptr) {
			root = new Node(key);
			return;
		}
        if (key < node->key) {
			if (node->left != nullptr) insert(node->left, key);
			else {
				node->left = new Node(key);
				node->left->parent = node;
			}
        } else if (key > node->key) {
			if (node->right != nullptr) insert(node->right, key);
			else {
				node->right = new Node(key);
				node->right->parent = node;
			}
		}
    }

    // Поиск минимального узла
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    // Удаление узла
    Node* remove(Node* node, int key) {
        if (!node) return nullptr;

        if (key < node->key)
            node->left = remove(node->left, key);
        else if (key > node->key)
            node->right = remove(node->right, key);
        else {
            // Узел с одним потомком или без потомков
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                // Узел с двумя потомками: находим минимальный узел в правом поддереве
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }

        return node;
    }

    // Поиск
    int& find(Node* node, int key) {
        if (!node) {
            static int empty;
            return empty;
        }

        if (key == node->key)
            return node->key;
        else if (key < node->key)
            return find(node->left, key);
        else
            return find(node->right, key);
    }

    // Проверка наличия ключа
    bool count(Node* node, int key) {
        if (!node) return false;
        if (key == node->key) return true;
        if (key < node->key) return count(node->left, key);
        return count(node->right, key);
    }

    // Рекурсивное удаление дерева
    void del(Node* node) {
        if (!node) return;
        del(node->left);
        del(node->right);
        delete node;
    }

	void order(Node* node, vector<int>& res) {
		if (node == nullptr) return;
		res.push_back(node->key);
		order(node->left, res);
		order(node->right, res);
	}

	int height(Node* node) {
		if (node == nullptr) return -1;
		return max(height(node->left), height(node->right))+1;
	}

	void generate(Node*& curr, int k, vector<int>& values, vector<pair<int, int> >& childs) {
		if (k == 0) return;

		curr = new Node(values[k-1]);
		if (k == 1) root = curr;

		generate(curr->left, childs[k-1].first, values, childs);
		generate(curr->right, childs[k-1].second, values, childs);
	}

	bool check(Node* curr, int min, int max) {
		if (curr == nullptr) return true;
		if (curr->key <= min || curr->key >= max) return false;

		return check(curr->left, min, curr->key) && check(curr->right, curr->key, max);
	}

public:
    Tree() : root(nullptr) {}

    ~Tree() {
        del(root);
    }

	int height() {return root ? height(root) : 0;}

    // Вставка ключа и значения
    void insert(int key) {
        insert(root, key);
    }

    // Удаление ключа
    void remove(int key) {
        root = remove(root, key);
    }

    // Поиск значения по ключу
    int& find(int key) {
        return find(root, key);
    }

    // Проверка наличия ключа
    bool count(int key) {
        return count(root, key);
    }

    // Оператор доступа
    int& operator[](int key) {
        if (!count(key)) {
            insert(key);
        }
        return find(key);
    }

	vector<int> order() {
		vector<int> res{};
		order(root, res);
		return res;
	}

	void generate(vector<int>& values, vector<pair<int, int> >& childs) {
		del(root);
		generate(root, 1, values, childs);
	}

	bool check(int c) {
		return check(root, -c, c);
	}

};

int main(void) {
	int n, buff, left, right;
	vector<int> vals;
	vector<pair<int, int>> childs;
	Tree array;

	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> buff;
		vals.push_back(buff);
	}

	for (int i = 0; i < n; ++i) {
		cin >> left >> right;
		childs.push_back(pair<int, int>(left, right));
	}

	array.generate(vals, childs);

	if (array.check(n+1)) cout << "Yes" <<endl;
	else cout << "No" << endl;
	
	return 0;
}

int max(int a, int b) {return (a > b ? a : b);}
int min(int a, int b) {return (a < b ? a : b);}
