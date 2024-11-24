#ifndef RedBlackTree
#define RedBlackTree

#include "EXCEPT.h"
#include <iostream>

namespace RBTREE {
    enum COLOR {RED, BLACK};

    using namespace PAIR;
    using namespace EXCEPT;

    template <typename K, typename V>
    class RBTree {

        class node {

            public:

            K key; V value;
            COLOR color;
            node* parent;
            node* left;
            node* right;
            static node null;

            node(K k = K(), V val = V(), COLOR c = RED) {
                key = k; value = val;
                color = c;
                parent = &null;
                left = &null;
                right = &null;
            }

            ~node(void) {
                parent = nullptr;
                left = nullptr;
                right = nullptr;
            }

            node& operator= (const node& n) {
                key = n.key; value = n.value;
                color = n.color;
                parent = n.parent;
                left = n.left;
                right = n.right;
            }

            const K& Key(void) const{
                return key;
            }

            const V& Value(void) const {
                return value;
            }

            node* brother(void) {
                return (parent->left == this) ? parent->right : parent->left;
            }

            node* grandparent(void) {
                return parent->parent;
            }

            node* uncle(void) {
                return parent->brother();
            }
            
            friend std::ostream& operator<< (std::ostream& os, const node& n) {
                return os << "\nKey: " << n->key << "\nValue: " << n->value << "\nColor: " << (n->color == RED) ? "RED\n" : "BLACK\n";
            }

            friend std::istream& operator>> (std::istream& is, node& n) {
                is >> n->key >>"\n">> n->value;
                if (!is.good()) throw Except("wrong input data");
                return is;
            }
        };

        node* root;
        size_t num;

        private:

        node* newNode(const K& key, const V& value) {
            ++num;
            node* res = new node(key, value);
            return res;
        }

        void delNode(node** n) {
            --num;
            delete *n;
        }

        void clear(node** n) {
            if (*n == &node::null) return;
            clear(&(*n)->left);
            clear(&(*n)->right);
            delNode(n);
        }

        node* rotateL(node* a) {
            auto b = a->right;
            auto med = b->left; 
            b->left = a;
            a->right = med;
            b->parent = a->parent;
            a->parent = b;
            return b;
        }

        node* rotateR(node* a) {
            auto b = a->left;
            auto med = b->right; 
            b->right = a;
            a->left = med;
            b->parent = a->parent;
            a->parent = b;
            return b;
        }
/*
        void insert_case1(node<K, V>* n);
        void insert_case2(node<K, V>* n);
        void insert_case3(node<K, V>* n);
        void insert_case4(node<K, V>* n);
        void insert_case5(node<K, V>* n);
*/
        void insert_case1(node* n) {
            if (n->parent == &node::null) n->color = BLACK;
            else insert_case2(n);
        }

        void insert_case2(node* n) {
            if (n->parent->color == BLACK) return;
            else insert_case3(n);
        }

        void insert_case3(node* n) {
            node* u = n->uncle(), *g = n->grandparent();
            if (u != &node::null && u->color == RED) {
                n->parent->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                insert_case1(g);
            } else insert_case4(n);
        }

        void insert_case4(node* n) {
            node* g = n->grandparent();
            if (n->parent->left == n && g->right == n->parent) n = rotateR(n->parent)->right;
            else if (n->parent->right == n && g->left == n->parent) n = rotateL(n->parent)->left;
            insert_case5(n);
        }

        void insert_case5(node* n) {
            node* g = n->grandparent();
            g->color = RED; n->parent->color = BLACK;
            if (g->left == n->parent) rotateR(n->parent);
            else rotateL(n->parent);
        }

        void insert(const K& key, const V& value, node** root) {
            node* n = *root;
            if (n == &node::null) *root = newNode(key, value);
            else {
                if (key == n->key) n->value = value;
                else if (key < n->key) insert(key, value, &n->left);
                else insert(key, value, &n->right);
            }
            insert_case1(n);
        }
/*
        void del_case1(node<K, V>* n);
        void del_case2(node<K, V>* n);
        void del_case3(node<K, V>* n);
        void del_case4(node<K, V>* n);
        void del_case5(node<K, V>* n);
        void del_case6(node<K, V>* n);
*/
        void replace(node* n, node* child) {
            child->parent = n->parent;
            if (n->parent->left == n) n->parent->left = child;
            else n->parent->right = child;
        }

        void delChild(node** no) {
            node* n = *no;
            node* child = (n->right == &node::null) ? n->left : n->right;
            replace(n, child);
            if (n->color == BLACK) {
                if (child->color == RED) child->color = BLACK;
                else del_case1(child);
            }
            delNode(no);
        }

        void del_case1(node* n) {
            if (n->parent != &node::null) {
                del_case2(n);
            }
        }

        void del_case2(node* n) {
            node* b = n->brother();
            if (b->color == RED) {
                n->parent->color = RED;
                b->color = BLACK;
                if (n->parent->left == n) rotateL(n->parent);
                else rotateR(n->parent);
            }
            del_case3(n);
        }

        void del_case3(node* n) {
            node* b = n->brother();
            if (n->parent->color == BLACK && b->color == BLACK && b->left->color == BLACK && b->right->color == BLACK) {
                b->color = RED;
                del_case1(n->parent);
            }
            else del_case4(n);
        }

        void del_case4(node* n) {
            node* b = n->brother();
            if (n->parent->color == RED && b->color == BLACK && b->left->color == BLACK && b->right->color == BLACK) {
                b->color = RED;
                n->parent->color = BLACK;
            } else del_case5(n);
        }

        void del_case5(node* n) {
            node* b = n->brother();
            if (n->parent->left == n && b->left->color == RED && b->right->color == BLACK) {
                b->color = RED;
                b->left->color = BLACK;
                rotateR(b);
            } else if (n->parent->right == n && b->left->color == BLACK && b->right->color == RED) {
                b->color = RED;
                b->right->color = BLACK;
                rotateL(b);
            }
            del_case6(n);
        }

        void del_case6(node* n) {
            node* b = n->brother();
            b->color = n->parent->color;
            n->parent->color = BLACK;

            if (n == n->parent->left) {
                b->right->color = BLACK;
                rotateL(n->parent);
            } else {
                b->left->color = BLACK;
                rotateR(n->parent);
            }
        }

        node* getMin(node* n) {
            node *res; res = n;
            if (res->left == &node::null) {
                res = res->right;
                while (res->left != &node::null) res = res->left;
            } else {
                res = res->left;
                while (res->right != &node::null) res = res->right;
            }
            return res;
        }

        void remove(node** root, const K& key) {
            node* n = *root;
            if (n->Key() == key) {
                if (n->left != &node::null && n->right != &node::null) {
                    node* m = getMin(n);
                    n->key = m->key; n->value = m->value;
                    delChild(&m);
                } else {
                    delChild(root);
                }

            } else if (n->Key() < key) {
                if (n->left == &node::null) return;
                remove(&n->left, key);
            } else {
                if (n->right == &node::null) return;
                remove(&n->right, key);
            }
        }

        public:

        RBTree(void) {
            num = 0;
            root = &(node::null);
        }

        RBTree(node* n) {
            root = n;
        }

        bool empty(void) {
            return num == 0;
        }

        size_t Num(void) {
            return num;
        }

        void clear(void) {
            clear(&root);
            root = &node::null;
        }

        ~RBTree(void) {
            this->clear();
        }

        bool find(const K& key, V& value) {
            node* n = root;
            while (n != &node::null) {
                if (n->Key() == key) {
                    value = n->value;
                    return true;
                }
                n = (n->Key() < key) ? n->left : n->right;
            }
            return false;
        }

        V& operator[] (const K& key) {
            node* n = root;
            while (n != &node::null) {
                if (n->Key() == key) {
                    return n->value;
                }
                n = (n->Key() < key) ? n->left : n->right;
            }
            throw Except("invalid key");
        }

        void insert(const K& key, const V& value) {
            insert(key, value, &root);
            root->color = BLACK;
        }

        void remove(const K& key) {
            remove(&root, key);
        }
    };
    template <typename K, typename V>
    typename RBTree<K, V>::node RBTree<K, V>::node::null(K(), V(), BLACK);
}

#endif
