#include <iostream>
#include <vector>
#include <string>
#include <functional>

struct TreeNode {
    char data;
    std::vector<TreeNode*> children;

    TreeNode(char val) : data(val) {}
};

class RollingHash {
private:
    const size_t p = 31;
public:
	const size_t m = 1e9 + 9;
	std::vector<size_t> p_pow;

    RollingHash(size_t max_len) {
        p_pow.resize(max_len + 1);
        p_pow[0] = 1;
        for (size_t i = 1; i <= max_len; ++i) {
            p_pow[i] = (p_pow[i - 1] * p) % m;
        }
    }

    size_t computeHash(const std::string &s) {
        size_t hash_value = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow[i]) % m;
        }
        return hash_value;
    }

    size_t rollingHash(size_t old_hash, char old_char, char new_char) {
        size_t new_hash = (old_hash - (old_char - 'a' + 1) + m) % m;
        new_hash = (new_hash * p) % m;
        new_hash = (new_hash + (new_char - 'a' + 1)) % m;
        return new_hash;
    }
};

void countPaths(TreeNode* node, const std::string &s, std::string &currentPath, RollingHash &rh, size_t &s_hash, size_t s_len, int &count) {
    if (node == nullptr) {
        return;
    }

    currentPath += node->data;

    if (currentPath.length() >= s_len) {
        size_t start_index = currentPath.length() - s_len;
        size_t substring_hash = 0;
        for (size_t i = 0; i < s_len; ++i) {
            substring_hash = (substring_hash + (currentPath[start_index + i] - 'a' + 1) * rh.p_pow[i]) % rh.m;
        }
        if (substring_hash == s_hash) {
            count++;
        }
    }

    for (TreeNode* child : node->children) {
        countPaths(child, s, currentPath, rh, s_hash, s_len, count);
    }

    currentPath.pop_back();
}

int countVerticalPaths(TreeNode* root, const std::string &s) {
    if (s.empty()) {
        return 0;
    }

    RollingHash rh(s.size());
    size_t s_hash = rh.computeHash(s);
    int count = 0;
    std::string currentPath;
    countPaths(root, s, currentPath, rh, s_hash, s.length(), count);
    return count;
}

int main() {
    // Пример дерева
    TreeNode* root = new TreeNode('a');
    root->children.push_back(new TreeNode('b'));
    root->children.push_back(new TreeNode('c'));
    root->children[0]->children.push_back(new TreeNode('a'));
    root->children[0]->children.push_back(new TreeNode('b'));
    root->children[1]->children.push_back(new TreeNode('d'));
    root->children[1]->children.push_back(new TreeNode('e'));

    std::string s;
    std::cin >> s;

    int count = countVerticalPaths(root, s);
    std::cout << "Количество вертикальных путей, содержащих строку: " << count << std::endl;

    return 0;
}

