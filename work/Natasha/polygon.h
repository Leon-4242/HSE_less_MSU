// класс многоугольников

#pragma once

#include <vector>
#include <algorithm>

class polygon {
    // class vertex
    class vertex {
        public:
        double x, y; //coordinates
        vertex (double x = 0, double y = 0) : x(x), y(y) {} //constructor
        ~vertex(void) {} //destructor
        vertex (const vertex& v): x(v.x), y(v.y) {} //copy constuctor
        vertex& operator= (const vertex& v) {x = v.x; y= v.y; return *this;} //copy operator
        bool operator< (const vertex& v) {return y < v.y;} // compare by y
        void print_as_pair(void) const; // вывод вершины на экран
        friend double area(const vertex& A, const vertex& B, const vertex& C) {return (B.x-A.x)*(C.y-A.y)-(B.y-A.y)*(C.x-A.x);} //area with sign
    };

    //class segment
    class segment {
        public:
        vertex a, b; //border points
        segment(vertex aa, vertex bb): a(aa), b(bb) {} //constuctor
        ~segment(void) {} //destructor
        segment(const segment& s): a(s.a), b(s.b) {} //copy constuctor
        segment& operator= (const segment& s) {a = s.a; b = s.b; return *this;} //copy operator
        friend bool insect_line_seg(const segment& line, const segment& s) {return area(line.a, line.b, s.a)*area(line.a, line.b, s.b) < 0;} //true <=> line intersects segment
        friend bool insect_seg_seg(const segment& s1, const segment& s2) {return insect_line_seg(s1, s2) && insect_line_seg(s2, s1);} //true <=> segments have same point
    };

    class intersect {
        public:
        bool f; // if (first polygon) true else false
        double d; // intersection point numerator
        double n; // intersection point denominator
        bool operator< (const intersect& in) const {return n*in.d<d*in.n;}
    };

    std::vector<vertex> vertices; // массив из вершин

    public:
        polygon(std::vector<vertex> newvertices): vertices(newvertices) {} // создать многоугольник
        ~polygon(void) {vertices.clear();} // удалить многоугольник

        size_t count_vertices(void) const {return vertices.size();} // посчитать количество вершин многоугольника
        bool empty(void) const {return vertices.empty();}

        const vertex& operator[] (const size_t& k) const {return vertices[k];}
        vertex& operator[] (const size_t& k) {return vertices[k];}
        void print_as_list(void) const; // вывести многоугольник на экран в виде списка вершин

        bool insect_sides(const polygon& p) const;
        size_t append_insects(std::vector<intersect>::iterator iter_insects, double y, bool flag) const;
        bool contains_in_stripe(const polygon& keypol, double y, std::vector<intersect>& insect) const;
        bool check_coverage(const polygon& keypol) const; // проверить вложение keypol в данный многоугольник
};
