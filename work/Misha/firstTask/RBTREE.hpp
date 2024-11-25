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
        Pair(const Pair& p): val1(p.val1), val2(p.val2) {}

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
    template <typename T, typename L>
    class PairTree {
        T val1; L val2; bool flag;
        public:
        PairTree(const T& v1 = T(), const L& v2 = L(), bool f = true): val1(v1), val2(v2), flag(f) {}
        PairTree(const PairTree& p): val1(p.val1), val2(p.val2), flag(p.flag) {}

        PairTree& operator= (const PairTree& p) {
            val1 = p.val1; val2 = p.val2; flag = p.flag; return *this;
        }

        ~PairTree(void) {}

        const T& first(void) const {return val1;}
        T& first(void) {return val1;}
        const L& second(void) const {return val2;}
        L& second(void) {return val2;}
        const bool& F(void) const {return flag;}

        bool operator== (const PairTree& p) const {
            return (val1 == p.val1) && (val2 == p.val2) && (flag == p.flag);
        }

        bool operator!= (const PairTree& p) const {
            return !(p == *this);
        }
    };

    template<>
    class PairTree<std::string, size_t> {
        std::string val1; size_t val2; bool flag;
        public:
        PairTree(const std::string v1 = "", const size_t& v2 = 0, bool f = true): val1(v1), val2(v2), flag(f) {}
        PairTree(const PairTree& p): val1(p.val1), val2(p.val2), flag(p.flag) {}

        PairTree& operator= (const PairTree& p) {
            val1 = p.val1; val2 = p.val2; flag = p.flag; 
            return *this;
        }

        PairTree& operator= (const size_t& value) {
            if (value < MROT) flag = false;
            val2 = value;
            return *this;
        }

        ~PairTree(void) {}

        const std::string& first(void) const {return val1;}
        std::string& first(void) {return val1;}
        const size_t& second(void) const {return val2;}
        size_t& second(void) {return val2;}
        const bool& F(void) const {return flag;}

        bool operator== (const PairTree& p) const {
            return (val1 == p.val1) && (val2 == p.val2) && (flag == p.flag);
        }

        bool operator!= (const PairTree& p) const {
            return !(p == *this);
        }

    };

    
    enum COLOR {RED, BLACK};
    using namespace EXCEPT;
//    using namespace PAIR;

    template <typename K, typename V>
    class RBTree {

        class node {

            public:

            PairTree<K, V> key_value;
            COLOR color;
            node* parent;
            node* left;
            node* right;
            static node null;

            node(const PairTree<K, V> k_v, const COLOR c = RED): key_value(k_v), color(c), parent(&null), left(&null), right(&null) {}

            ~node(void) {
                parent = nullptr;
                left = nullptr;
                right = nullptr;
            }

            node& operator= (const node& n) {
                key_value = n.key_value;
                color = n.color;
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

            friend std::ostream& operator<< (std::ostream& os, const node* n) {
                if (n == &node::null) return os << "\nLIST\n";
                if (!n->key_value.F()) return os;
                return os << "\nKey: " << n->Key() << " Value: " << n->Value() << "\n";
            }

            friend std::istream& operator>> (std::istream& is, node& n) {
                is >> n->Key() >>"\n">> n->Value();
                if (!is.good()) throw Except("wrong input data");
                return is;
            }

            bool operator== (const node& n) const {
                return (n.key_value == key_value);
            }

            bool operator!= (const node& n) const {
                return !(*this == n);
            }
        };

        node* root;
        size_t num;

        private:

        node* newNode(const K& key, const V& value) {
            ++num;
            node* res = new node(PairTree<K, V>(key, value));
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
                std::cout <<"321" << std::endl;
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

        void delChild(node** root) {
        	node* n = *root;
            node* child = (n->right == &node::null) ? n->left : n->right;
			replace(n, child);
            if (n->color == BLACK) {
                if (child->color == RED) child->color = BLACK;
                else del_case1(child);
            }
            delNode(root);
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

        node* newTree(const node* root) {
            if (root == &node::null) return &node::null;
            node* newRoot = new node(PairTree<K, V>(root->Key(),root->Value()), root->Color());
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
            print(os, n->left) << n;
            print(os, n->right);
            return os;
        }

        std::ostream& print(std::ostream& os, const node* n, const K& k1, const K& k2) const {
            if (n == &node::null) return os;
            if (n->Key() < k1 || n->Key() > k2) return os;
            print(os, n->left, k1, k2) << n;
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

        bool find(PairTree<K, V>& p) {
            node* n = root;
            while (n != &node::null) {
                if (n->Key() == p.first()) {
                    p.second() = n->Value();
                    return true;
                }
                n = (n->Key() < p.first()) ? n->left : n->right;
            }
            return false;
        }

        PairTree<K, V>& operator[] (const K& key) {
            node* n = root;
            while (n != &node::null) {
                if (n->Key() == key) {
                    return n->key_value;
                }
                n = (n->Key() > key) ? n->left : n->right;
            }
            throw Except("invalid key");
        }

        void insert(const K& key, const V& value) {
            insert(key, value, &root);
            root->color = BLACK;
        }

        void insert(const PairTree<K, V>& p) {
            insert(p.first(), p.second(), &root);
            root->color = BLACK;
        }

        void remove(const K& key) {
            remove(&root, key);
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
    typename RBTree<K, V>::node RBTree<K, V>::node::null(PairTree<K, V>(), BLACK);
}

#endif
