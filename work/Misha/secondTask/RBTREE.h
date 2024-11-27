#ifndef RedBlackTree
#define RedBlackTree

#include "EXCEPT.h"
#include <iostream>

namespace RBTREE {
    enum COLOR {RED, BLACK};
    using namespace EXCEPT;

    template <typename K, typename V>
    class RBTree {
        protected:
        class node {

            public:

            K key; V value;
            COLOR color;
            node* parent;
            node* left;
            node* right;
            static node null;

            node(K k = K(), V val = V(), COLOR c = RED): key(k), value(val), color(c), parent(&null), left(&null), right(&null){}

            ~node(void) {
                parent = nullptr;
                left = nullptr;
                right = nullptr;
            }

            node& operator= (const node& n) {
                key = n.key; value = n.value;
                color = n.color;
            }

            const K& Key(void) const{return key;}
            K& Key(void) {return key;}
            const V& Value(void) const {return value;}
            V& Value(void) {return value;}
            const COLOR& Color(void) const {return color;}
            COLOR& Color(void) {return color;}

            node* brother(void) {return (parent->left == this) ? parent->right : parent->left;}

            node* grandparent(void) {return parent->parent;}

            node* uncle(void) {return parent->brother();}

            friend std::ostream& operator<< (std::ostream& os, const node& n) {
                if (&n == &node::null) return os << "\nLIST\n";
                return os << "\nKey: " << n.Key() << " Value: " << n.Value() << "\n";
            }

            friend std::istream& operator>> (std::istream& is, node& n) {
                is >> n.key >>"\n">> n.value;
                if (!is.good()) throw Except("wrong input data");
                return is;
            }
            
            bool operator== (const node& n) const {
                return (n.key == key) && (n.value == value);
            }

            bool operator!= (const node& n) const {
                return !(*this == n);
            }
        };

        node* root;
        size_t num;

        node* newNode(const K& key, const V& value) {
            ++num;
            node* res = new node(key, value);
            return res;
        }

        void delNode(node*& n) {
            --num;
            delete n;
            n = &node::null;
        }

        void clear(node* n) {
            if (n == &node::null) return;
            clear(n->left);
            clear(n->right);
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

        node* insert(const K& key, const V& value, node*& n) {
            if (n == &node::null) {
                n = newNode(key, value);
                return n;
            } else {
                if (key == n->Key()) n->Value() = value;
                else if (key < n->Key()) {
                    node *tmp = insert(key, value, n->left);
                    tmp->parent = n;
                } else {
                    node *tmp = insert(key, value, n->right);
                    tmp->parent = n;
                }
            }
            insert_case1(n);
            return n;
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
            else if (n->parent->right == n) n->parent->right = child;
        }

        void delChild(node* n) {
            node* child = (n->right == &node::null) ? n->left : n->right;
            replace(n, child);
            if (n->color == BLACK) {
                if (child->color == RED) child->color = BLACK;
                else del_case1(child);
            }
            delNode(n);
        }

        void del_case1(node* n) {
            if (n->parent != &node::null) del_case2(n);
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

        void remove(node* n, const K& key) {
            if (n->Key() == key) {
                if (n->left != &node::null && n->right != &node::null) {
                    node* m = getMin(n);
                    n->key = m->key; n->value = m->value;
                    delChild(m);
                } else {
                    delChild(n);
                }

            } else if (n->Key() > key) {
                if (n->left == &node::null) return;
                remove(n->left, key);
            } else {
                if (n->right == &node::null) return;
                remove(n->right, key);
            }
        }
		
		node* newTree(const node* root) {
            if (root == &node::null) return &node::null;
            node* newRoot = new node(root->Key(),root->Value(), root->Color());
            newRoot->left = newTree(root->left); newRoot->right = newTree(root->right);
            newRoot->left->parent = newRoot;
            newRoot->right->parent = newRoot;
            return newRoot;
        }
        
        void add(const node* n) {
            if (n == &node::null) return;
            insert(n->Key(), n->Value());
            add(n->left);
            add(n->right);
        }
        
        std::ostream& print(std::ostream& os, const node* n) const {
            if (n == &node::null) return os;
            print(os, n->left) <<*n;
            print(os, n->right);
            return os;
        }
        
        std::ostream& print(std::ostream& os, const node* n, const K& k1, const K& k2) const {
            if (n == &node::null) return os;
            if (n->Key() < k1 || n->Key() > k2) return os;
            print(os, n->left, k1, k2) << *n;
            print(os, n->right, k1, k2);
            return os;
        }

		bool eq(const node* n1, const node* n2) const {
            if (n1 == &node::null && n2 == &node::null) return true;
            if ((n1 == &node::null && n2 != &node::null) || (n1 != &node::null && n2 == &node::null)) return false;
            return (*n1 == *n2) && eq(n1->left, n2->left) && eq(n1->right, n2->right);
        }
        
		public:

        RBTree(void) {
            num = 0;
            root = &node::null;
        }

        RBTree(node* n) {
            root = n;
        }

		RBTree(const RBTree& tree) {
            num = tree.num;
            root = newTree(tree.root);
        }
        
        RBTree& operator= (const RBTree& tree) {
            this->clear();
            num = tree.num;
            root = newTree(tree.root);
            return *this;
        }
        
        bool empty(void) {
            return num == 0;
        }

        size_t Num(void) {
            return num;
        }

        void clear(void) {
            clear(root);
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
                n = (n->Key() > key) ? n->left : n->right;
            }
            return false;
        }

        V& operator[] (const K& key) {
            node* n = root;
            while (n != &node::null) {
                if (n->Key() == key) {
                    return n->value;
                }
                n = (n->Key() > key) ? n->left : n->right;
            }
            throw Except("invalid key");
        }

        void insert(const K& key, const V& value) {
            insert(key, value, root);
            root->color = BLACK;
        }

        void remove(const K& key) {
            remove(root, key);
        }
        
        friend std::ostream& operator<< (std::ostream& os, const RBTree<K, V>& tree) {
            return tree.print(os, tree.root);
        }
        
		std::ostream& Print(std::ostream& os, const K& k1, const K& k2) {
            return print(os, root, k1, k2);
        }
        
        RBTree operator+ (const RBTree& tree) const {
            RBTree res(*this);
            res.add(tree.root); return res;
        }

        RBTree& operator+= (const RBTree tree) {
            *this = *this + tree;
            return *this;
        }

        bool operator== (const RBTree& tree) const {
            return eq(root, tree.root);
        }

        bool operator!= (const RBTree& tree) const {
            return !(*this == tree);
        }
    };
    template <typename K, typename V>
    typename RBTree<K, V>::node RBTree<K, V>::node::null(K(), V(), BLACK);
}

#endif
