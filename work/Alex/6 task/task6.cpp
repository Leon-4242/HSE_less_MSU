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
		int size;
        Node* left;
        Node* right;
		Node* parent;

        Node(int k)
            : key(k), size(1), left(nullptr), right(nullptr), parent(nullptr) {}
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
		
		if(y->left != nullptr) {
			int all = y->size;
			y->size = y->size - x->size + y->left->size;
			x->size = all;
		}

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

		if(x->right != nullptr) {
			int all = x->size;
			x->size = x->size - y->size + x->right->size;
			y->size = all;
		}

        return y;
    }

    // Вставка
    void insert(Node* node, int key) {
		if (root == nullptr) {
			root = new Node(key);
			return;
		}

        if (key < node->key) {
			if (node->left != nullptr) {
				insert(node->left, key);
				++node->size;
			} else {
				node->left = new Node(key);
				node->left->parent = node;
				++node->size;
			}
        } else if (key > node->key) {
			if (node->right != nullptr) {
				insert(node->right, key);
				++node->size;
			} else {
				node->right = new Node(key);
				node->right->parent = node;
				++node->size;
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

	int mediana(int n) {
		int mid = (n+1)/2 - 1;
		int num = (root->left) ? root->left->size : 0;
		Node* curr = root;

		while (num != mid) {
			if (num > mid) {
				num -= (curr->left->right ? curr->left->right->size : 0) + 1;
				curr = curr->left; 	
			} else /*(num < mid)*/ {
				num += (curr->right->left ? curr->right->left->size : 0) + 1;
				curr = curr->right;
			}
		}

		return curr->key;
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

	int mediana() {
		return mediana(root->size);
	}
};

int main(void) {
	int n, buff;
	Tree array; vector<int> res;

	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> buff;
		array.insert(buff);
		res.push_back(array.mediana());
	}

	for (auto it = res.begin(); it != res.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	
	return 0;
}

int max(int a, int b) {return (a > b ? a : b);}
int min(int a, int b) {return (a < b ? a : b);}
