#ifndef LIB_MED
#define LIB_MED

#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>

using point = struct Point {
    double x, y;
};

using circle = struct Circle {
    point c;
    double r;
};

void shuffle_vector(std::vector<point> &v);

double dist(const point &a, const point &b);

circle circle_from_2(const point &a, const point &b);

circle circle_from_3(const point &a, const point &b, const point &c);

bool is_inside(const point &p, const circle &C);

circle welzl(std::vector<point> &pts, std::vector<point> &R, int n);

circle minimal_enclosing_circle(std::vector<point> points);

#endif
