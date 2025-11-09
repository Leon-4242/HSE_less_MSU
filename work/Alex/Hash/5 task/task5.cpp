#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class SubstringInfo {
    int l;
    int r;
    std::string substring;

    SubstringInfo(int l, int r, const std::string &s) : l(l), r(r), substring(s.substr(l, r - l + 1)) {}
};

std::vector<SubstringInfo> sortSubstringsLexicographically(const std::string &s, const std::vector<std::pair<int, int>> &indices) {
    std::vector<SubstringInfo> substrings;

    // Заполняем вектор подстрок
    for (const auto &index : indices) {
        substrings.emplace_back(index.first, index.second, s);
    }

    // Сортируем подстроки лексикографически
    std::sort(substrings.begin(), substrings.end(), [](const SubstringInfo &a, const SubstringInfo &b) {
        return a.substring < b.substring;
    });

    return substrings;
}

int main() {
    std::string s;
    std::cin >> s;

    int n;
    std::cin >> n;

    std::vector<std::pair<int, int>> indices(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> indices[i].first >> indices[i].second;
    }

    std::vector<SubstringInfo> sortedSubstrings = sortSubstringsLexicographically(s, indices);

    for (const auto &substringInfo : sortedSubstrings) {
        std::cout << substringInfo.substring << std::endl;
    }

    return 0;
}

