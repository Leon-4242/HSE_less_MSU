#include <iostream>
#include <vector>
#include <string>

int min(const int&, const int&);
int longestPalindrome(const std::string&);

int main() {
    std::string s;
    std::cin >> s;

    int max_palindrome_length = longestPalindrome(s);
    std::cout << "Длина самого длинного палиндрома: " << max_palindrome_length << std::endl;

    return 0;
}

// Алгоритм Манакера
int longestPalindrome(const std::string &s) {
    if (s.empty()) return 0;

    // Преобразуем строку для обработки четных палиндромов
    std::string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }

    int n = t.size();
    std::vector<int> p(n, 0);
    int center = 0, right = 0;
    int max_len = 0;

    for (int i = 0; i < n; ++i) {
        // Определяем начальное значение p[i]
        if (i < right) {
            p[i] = std::min(right - i, p[2 * center - i]);
        }

        // Расширяем палиндром с центром в i
        int left_bound = i - (1 + p[i]);
        int right_bound = i + (1 + p[i]);
        while (left_bound >= 0 && right_bound < n && t[left_bound] == t[right_bound]) {
            p[i]++;
            left_bound--;
            right_bound++;
        }

        // Обновляем центр и правую границу
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }

        // Обновляем максимальную длину палиндрома
        if (p[i] > max_len) {
            max_len = p[i];
        }
    }

    return max_len;
}

int min(const int& a, const int &b) {return  (a < b ? a : b);}

