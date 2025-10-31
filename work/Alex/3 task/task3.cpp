#include <iostream>
#include <vector>

using namespace std;

int max(int, int);

class AVLTree {
private:
    class Node {
    public:
        int key;
        int value;
        Node* left;
        Node* right;
        int height;

        Node(int k, const int& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
        ~Node() {
            left = nullptr;
            right = nullptr;
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
    Node* insert(Node* node, int key, const int& value) {
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
                node->value = temp->value;
                node->right = remove(node->right, temp->key);
            }
        }

        if (!node) return node;

        return balance(node);
    }

    // Поиск
    int& find(Node* node, int key) {
        if (!node) {
            static int empty;
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
    void insert(int key, const int& value = {}) {
        root = insert(root, key, value);
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
            insert(key, int{});
        }
        return find(key);
    }

};


int main(void) {
	int q, type, buff;
	AVLTree array;

	cin >> q;
	for (int i = 0; i < q; ++i) {
		cin >> type;
		cin	>> buff;
		if (type == 1) {
			if (array.count(buff)) cout << 0 << endl;
			else {array.insert(buff, buff); cout << 1 << endl;}
		} else if (type == 2) {
			if (!array.count(buff)) cout << 0 << endl;
			else {array.remove(buff); cout << 1 << endl;}
		} else if (type == 3){
			cout << array.count(buff) << endl;
		}
	}

	return 0;
}

int max(int a, int b) {return (a > b ? a : b);}
