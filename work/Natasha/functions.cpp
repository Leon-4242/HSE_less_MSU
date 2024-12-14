#include <iostream>
#include <cmath>
#include "polygon.h"

void polygon::vertex::print_as_pair(void) const{ // вывод вершины на экран
    std::cout << x << ", " << y << std::endl;
}

void polygon::print_as_list(void) const { // вывести pol на экран в виде списка вершин
    if (vertices.empty() == false) for (size_t i = 0; i < vertices.size(); ++i) vertices[i].print_as_pair();
    else std::cout << "polygon is blank" << std::endl;
}

bool polygon::insect_sides(const polygon& p) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = 0; j < p.count_vertices(); ++j) {
            if (insect_seg_seg(
                segment(vertices[i], vertices[(i == vertices.size()-1) ? (0) : (i+1)]), 
                segment(p[j], p[(j == p.count_vertices()-1) ? (0) : (j+1)])))
            return true;
        }
    }

    return false;
}

size_t polygon::append_insects(std::vector<intersect>::iterator iter_insects, double y, bool flag) const{
    size_t n = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex p1 = vertices[i];
        vertex p2 = vertices[(i == vertices.size()-1) ? (0) : (i+1)];
        if (p2 < p1) {
            auto tmp(p1);
            p1 = p2;
            p2 = tmp;
        }

        if (p1.y < y && y < p2.y) {
            (*(iter_insects+n)).f = flag;
            (*(iter_insects+n)).n = p2.x * (y - p1.y) + p1.x * (p2.y - y);
            (*(iter_insects+n)).d = p2.y - p1.y;
            ++n;
        }
    }
    return n;
}

bool polygon::contains_in_stripe(const polygon& keypol, double y, std::vector<intersect>& insects) const {
    size_t n = append_insects(insects.begin(), y, true);
    n += keypol.append_insects(insects.begin()+n, y, false);
    sort(insects.begin(), insects.end());
    bool f1 = false, f2 = false;
    for (size_t i = 0; i < n; ++i) {
        if (insects[i].f) f1 = (!f1 && insects[i].f) || (f1 && !insects[i].f);
        else f2 = (!f2 && insects[i].f) || (f2 && !insects[i].f);
        if (!f1 && f2) return false;
    }
    return true;
}

bool polygon::check_coverage(const polygon& keypol) const {// проверить вложение keypol в pol (метод суммирования углов)
    if (insect_sides(keypol)) return false;
    std::vector<vertex> points_sort_y(vertices); points_sort_y.insert(points_sort_y.end(), keypol.vertices.begin(), keypol.vertices.end());
    sort(points_sort_y.begin(), points_sort_y.end());
    std::vector<intersect> insects(points_sort_y.size());
    for (size_t i = 1; i < points_sort_y.size(); ++i) {
        double y = (points_sort_y[i-1].y + points_sort_y[i].y)/2;
        if (!this->contains_in_stripe(keypol, y, insects)) return false;
    }
    return true;
}
