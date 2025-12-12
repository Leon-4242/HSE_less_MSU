#include "lib.hpp"

void shuffle_vector(std::vector<point> &v) {
    for (int i = v.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
		std::swap(v[i], v[j]);
    }
}

double dist(const point &a, const point &b) {
    return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

circle circle_from_2(const point &a, const point &b) {
    point c;
    c.x = (a.x + b.x) / 2.0;
    c.y = (a.y + b.y) / 2.0;
    double r = dist(a, b) / 2.0;
    return {c, r};
}

circle circle_from_3(const point &a, const point &b, const point &c) {
    double d = 2 * (a.x*(b.y - c.y) +
                    b.x*(c.y - a.y) +
                    c.x*(a.y - b.y));

    if (fabs(d) < 1e-16) {
        return {{0,0}, -1}; // invalid
    }

    double a2 = a.x*a.x + a.y*a.y;
    double b2 = b.x*b.x + b.y*b.y;
    double c2 = c.x*c.x + c.y*c.y;

    double ux = (a2*(b.y - c.y) +
                 b2*(c.y - a.y) +
                 c2*(a.y - b.y)) / d;

    double uy = (a2*(c.x - b.x) +
                 b2*(a.x - c.x) +
                 c2*(b.x - a.x)) / d;

    point center{ux, uy};
    double r = dist(center, a);

    return {center, r};
}

bool is_inside(const point &p, const circle &C) {
    return dist(p, C.c) <= C.r + 1e-12;
}

circle welzl(std::vector<point> &pts, std::vector<point> &R, int n) {
    if (n == 0 || R.size() == 3) {
        if (R.empty())
            return {{0,0}, -1};
        else if (R.size() == 1)
            return {R[0], 0.0};
        else if (R.size() == 2)
            return circle_from_2(R[0], R[1]);
        else
            return circle_from_3(R[0], R[1], R[2]);
    }

    int idx = rand() % n;
    point p = pts[idx];
	std::swap(pts[idx], pts[n - 1]);

    circle D = welzl(pts, R, n - 1);

    if (D.r < 0 || !is_inside(p, D)) {
        R.push_back(p);
        D = welzl(pts, R, n - 1);
        R.pop_back();
    }

    return D;
}

circle minimal_enclosing_circle(std::vector<point> points) {
    shuffle_vector(points);
	std::vector<point> R;
    return welzl(points, R, points.size());
}
