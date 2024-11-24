#ifndef RedBlackTree
#define RedBlackTree

#include "EXCEPT.hpp"
#include <iostream>

namespace PAIR {
    template <typename T, typename L>
    class Pair {
        T val1; L val2;
        public:
        Pair(const T& v1 = T(), const L& v2 = L()): val1(v1), val2(v2) {}
        Pair(const Pair& p) val1(p.val1), val2(p.val2) {}

        Pair& operator= (const Pair& p) {
            val1 = p.val1; val2 = p.val2; return *this;
        }

        ~Pair(void) {}

        const T& first(void) const {return val1;}
        T& first(void) {return val1;}

        const L& second(void) const {return val2;}
        L& second(void) {return val2;}
    };
}

namespace RBTREE {
    enum COLOR {RED, BLACK};
    using namespace EXCEPT;
    using namespace PAIR;

    template <typename K, typename V>
    class RBTree {

        class node {

            public:

            Pair<K, V> key_value;
            COLOR color;
            node* parent;
            node* left;
            node* right;
            static node null;

            node(const K k = K(), const V val = V(), const COLOR c = RED): key_value(k, val), color(c), parent(&null), left(&null), right(&null) {}

            ~node(void) {
                parent = nullptr;
                left = nullptr;
                right = nullptr;
            }

            node& operator= (const node& n) {
                key_value = n.key_value;
                color = n.color;
                parent = n.parent;
                left = n.left;
                right = n.right;
            }

            const K& Key(void) const {return key_value.first();}
            K& Key(void) {return key_value.first();}

            const V& Value(void) const {return key_value.second();}
            V& Value(void) {return key_value.second();}

            const COLOR& Color(void) const {return color;}
            COLOR& Color(void) {return color;}

            node* brother(void) {return (parent->left == this) ? parent->right : parent->left;}

            node* grandparent(void) {return parent->parent;}

            node* uncle(void) {return parent->brother();}

            friend std::ostream& operator<< (std::ostream& os, const node& n) {
                if (n == null) return os << "\nLIST\n";
                return os << "\nKey: " << n->key << " Value: " << n->value << "\n";
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
                if (key == n->Key()) n->Value() = value;
                else if (key < n->Key()) insert(key, value, &n->left);
                else insert(key, value, &n->right);
            }
            insert_case1(n);
        }



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
                    n->Key() = m->Key(); n->Value() = m->Value();
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

        node* newTree(const node* root) const{
            if (root == &node::null) return &node::null;
            node* newRoot, *newL = newTree(root->left), *newR = newTree(root->right);
            newRoot = new node(root->Key(), root->Value(), root->Color());
            newRoot->left = newL; newRoot->right = newR;
            newL->parent = newR->parent = newRoot;
            return newRoot;
        }

        void add(const node* n) {
            if (n == &node::null) return;
            insert(n->Key(), n->Value());
            add(n->left);
            add(n->right);
        }

        public:

        RBTree(void) {
            num = 0;
            root = &(node::null);
        }

        RBTree(node* n) {
            root = n;
        }

        RBTree(const RBTree& tree) {
            num = tree.num;
            root = tree.newTree(tree.root);
        }

        RBTree& operator= (const RBTree& tree) {
            this->clear();
            num = tree.num;
            root = tree.newTree(tree.root);
            return *this;
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
        friend std::ostream& operator<< (std::ostream& os, const RBTree<K, V>& tree) {
            if (tree.root == &node::null) return os;
            return os << RBTree<K, V>(tree.root->left) << tree.root << RBTree<K, V>(tree.root->right);
        }

        RBTree operator+ (const RBTree& tree) const {
            RBTree res(*this);
            res.add(tree.root); return res;
        }
    };
    template <typename K, typename V>
    typename RBTree<K, V>::node RBTree<K, V>::node::null(K(), V(), BLACK);
}

#endif
