#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

struct Point {
    double x, y;
};

struct Circle {
    Point c;
    double r;
};

void shuffle_vector(vector<Point> &v) {
    for (int i = v.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(v[i], v[j]);
    }
}

double dist(const Point &a, const Point &b) {
    return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

Circle circle_from_2(const Point &a, const Point &b) {
    Point c;
    c.x = (a.x + b.x) / 2.0;
    c.y = (a.y + b.y) / 2.0;
    double r = dist(a, b) / 2.0;
    return {c, r};
}

bool collinear(const Point &a, const Point &b, const Point &c) {
    double d = a.x*(b.y - c.y) +
               b.x*(c.y - a.y) +
               c.x*(a.y - b.y);
    return fabs(d) < 1e-14;
}

Circle circle_from_3(const Point &a, const Point &b, const Point &c) {
    double d = 2 * (a.x*(b.y - c.y) +
                    b.x*(c.y - a.y) +
                    c.x*(a.y - b.y));

    if (fabs(d) < 1e-14) {
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

    Point center{ux, uy};
    double r = dist(center, a);

    return {center, r};
}

bool is_inside(const Point &p, const Circle &C) {
    return dist(p, C.c) <= C.r + 1e-12;
}

Circle welzl(vector<Point> &pts, vector<Point> &R, int n) {
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
    Point p = pts[idx];
    swap(pts[idx], pts[n - 1]);

    Circle D = welzl(pts, R, n - 1);

    if (D.r < 0 || !is_inside(p, D)) {
        R.push_back(p);
        D = welzl(pts, R, n - 1);
        R.pop_back();
    }

    return D;
}

Circle minimal_enclosing_circle(vector<Point> points) {
    shuffle_vector(points);
    vector<Point> R;
    return welzl(points, R, points.size());
}

int main() {
    vector<Point> points;
    int answer;
    cout << "Work with file?\n1)Yes\n2)No\n";
    cin >> answer;

    if (answer == 2) {
        int n;
        cout << "Enter number of points: ";
        cin >> n;
        points.resize(n);
        for (int i = 0; i < n; i++) {
            cin >> points[i].x >> points[i].y;
        }
    } else if (answer == 1){
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        ifstream f(filename);
        if (!f) {
            cerr << "File not found.\n";
            return 1;
        }
        Point p;
        while (f >> p.x >> p.y) {
            points.push_back(p);
        }
    }
	else 
	{
		return 0;
	}

    Circle C = minimal_enclosing_circle(points);

    cout << "Center: (" << C.c.x << ", " << C.c.y << ")\n";
    cout << "Radius: " << C.r << "\n";

    return 0;
}

