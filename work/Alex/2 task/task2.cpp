#include <iostream>
#include <vector>
#include <map>

using namespace std;

int max(int, int);

class AVLTree {
private:
    class Node {
    public:
        int key;
        vector<int> value;
        Node* left;
        Node* right;
        int height;

        Node(int k, const vector<int>& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
        ~Node() {
            left = nullptr;
            right = nullptr;
            value.clear();
        }
    };

    Node* root;

    // Вспомогательные функции
    inline int height(Node* node) {
        return node ? node->height : 0;
    }

    inline int balanceFactor(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    inline void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + max(height(node->left), height(node->right));
        }
    }

    // Повороты
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Балансировка
    Node* balance(Node* node) {
        if (!node) return nullptr;

        updateHeight(node);
        int bf = balanceFactor(node);

        // Left Left Case
        if (bf > 1 && balanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Right Right Case
        if (bf < -1 && balanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Left Right Case
        if (bf > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Вставка
    Node* insert(Node* node, int key, const vector<int>& value) {
        if (!node) return new Node(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else {
            // Ключ уже существует, обновляем значение
            node->value = value;
            return node;
        }

        return balance(node);
    }

    // Поиск
    vector<int>& find(Node* node, int key) {
        if (!node) {
            static vector<int> empty;
            return empty;
        }

        if (key == node->key)
            return node->value;
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

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        del(root);
    }

    // Вставка ключа и значения
    void insert(int key, const vector<int>& value = {}) {
        root = insert(root, key, value);
    }

    // Поиск значения по ключу
    vector<int>& find(int key) {
        return find(root, key);
    }

    // Проверка наличия ключа
    bool count(int key) {
        return count(root, key);
    }

    // Оператор доступа
    vector<int>& operator[](int key) {
        if (!count(key)) {
            insert(key, vector<int>());
        }
        return find(key);
    }

};


void count (const int& lv, const int& k, AVLTree& rev_array, vector<int>& table);

int main(void) {
	int n, buff;
	vector<int> array;
	AVLTree rev_array;

	cin >> n;
	for (int i = 0; i < n-1; ++i) {
		cin >> buff;
		array.push_back(buff);
	}

	for (int i = 0; i < n-1; ++i) rev_array[array[i]] = vector<int>();

	for (int i = 0; i < n-1; ++i) {
		rev_array[array[i]].push_back(i+2);
	}

	vector<int> table(n, 0);

	count(1, 0, rev_array, table);

	int k = 0;
	for (int i = 1; i < n; ++i) {
		if (table[i] > k) k = table[i];
	}

	vector<int> res(k+1, 0);
	for (int i = 0; i < n; ++i) {
		++res[table[i]];
	}

	cout << k << endl;
	for (int i = 0; i < k+1; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
	return 0;
}

void count (const int& lv, const int& k, AVLTree& rev_array, vector<int>& table) {
	table[lv-1] = k;
	if (!rev_array.count(lv)) return;
	
	auto vect = rev_array[lv];
	for (auto it = vect.begin(); it != vect.end(); ++it) count(*it, k+1, rev_array, table);
}

int max(int a, int b) {return (a > b ? a : b);}
