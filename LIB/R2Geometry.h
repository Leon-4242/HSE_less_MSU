#ifndef GEOMETRY
#define GEOMETRY

#include <iostream>
#include <cmath>

#define EPS 1e-12

    Vector operator- (const Vector& u, const Vector& v) {
        return Vector(u.X()-v.X(), u.Y()-v.Y());
    }

    Point operator+ (const Vector& v, const Point& p) {
        return Point(v.X()+p.X(), v.Y()+p.Y());
    }

    Vector operator+(const Vector& v) {
        return Vector(v.X(), v.Y());
    }

    Vector operator-(const Vector& v) {
        return Vector(-v.X(), -v.Y());
    }

    double operator*(const Vector& u, const Vector v) {
        return u.X()*v.X()+u.Y()*v.Y();
    }

    Vector operator*(const Vector& v, const double c) {
        return Vector(c*v.X(), c*v.Y());
    }

    Vector operator*(const double c, const Vector& v) {
        return Vector(c*v.X(), c*v.Y());
    }

    Point operator* (const double c, const Point& p) {
        return Point(c*p.X(), c*p.Y());
    }

    Point operator* (const Point& p, const double c) {
        return Point(c*p.X(), c*p.Y());
    }

    Vector RVector (const Point& p) {
        return Vector(p.X(), p.Y());
    }

    Point operator+ (const Point& p, const Vector& v) {
        return Point(p.X()+v.X(), p.Y()+v.Y());
    }

    Point operator+ (const Point& p, const Point& q) {
        return Point (q.X() + p.X(), q.Y() + p.Y());
    }

    Vector operator- (const Point& p, const Point& q) {
        return Vector(p.X()-q.X(), p.Y()-q.Y());
    }

    Point RPoint (const Vector& v) {
        return Point(v.X(), v.Y());
    }

namespace Geometry {
    template <typename T, size_t n>
    class 
    template <typename T>
    class Point;

    template <typename T>
    class Vector {
        std::vector<T> data;
    }

    Vector operator*(const double c, const Vector& v);

    Point operator* (const double c, const Point& p);

    template <typename T>
    class Point {
        template <typename K>
        friend class Vector;

        std::vector<T> data;
        public:
    
        Point (const double c = 0.) {
            double tmp = 0.;
            if (!(fabs(c) < EPS)) tmp = c;
            data(n, tmp);
        }
        
        Point (const std::vector<T> value) {
            data.assign(value.begin(), value.end());
        }

        Point(const Point& p) {
            data.assign(p.data.begin(), p.data.end());
        }

        Point(Point&& p) {
            data.clear();
            data.swap(p.data);
        }

        Point& operator= (const Point& p) {
            data.assign(p.data.begin(), p.data.end());
            return *this;
        }

        Point& operator= (Point&& p) {
            data.clear();
            data.swap(p.data);
            return *this;
        }
    
        T& operator[] (const size_t k) {
            if(k >= data.length()) throw Except("out of range");
            return data[k];
        }

        T operator[] (const size_t k) const {
            if (k >= data.length()) throw Except("out of range");
            return data[k];
        }

        Point operator+ (const Point<T>& p) const {
            if (data.length() != p.data.length()) throw Except("not same dimensions");
            Point<T> res;
            for (size_t i = 0; i < data.length; ++i) res.data[i] =data[i] + p.data[i];
            return res;
        }

        Point& operator+= (const Point<T>& p) {
            *this = *this + p;
            return *this;
        }

        Point operator+ (const Vector<T>& v) const {
            if (data.length() != v.data.length()) throw Except("not same dimensions");
            Point<T> res;
            for (size_t i = 0; i < data.length(); ++i) res.data[i] = data[i] + v.data[i];
        }

        Point& operator+= (const Vector<T>& v) {
            *this = *this + v;
            return *this;
        }

    	Vector operator- (const Point& p, const Point& q);
        Point operator* (const Point& p, const double c);
        Vector toVector (const Point& p);
	};

    class Vector {
        double x;
        double y;

        public:

        Vector(double xx = 0, double yy = 0) {
            if (fabs(xx) < EPS) x = 0;
            else x = xx;

            if (fabs(yy) < EPS) y = 0;
            else y = yy;
        }

        Vector(const Vector& v) {
            x = v.x;
            y = v.y;
        }

        Vector(Vector&& v) {
            x = v.x; v.x = 0;
            y = v.y; v.y = 0;
        }

        Vector& operator= (const Vector& v) {
            x = v.x;
            y = v.y;
            return *this;
        }
    
        Vector& operator= (Vector&& v) {
            x = v.x; v.x = 0;
            y = v.y; v.y = 0;
            return *this;
        }

        double X(void) const{
            return x;
        }

        double Y(void) const{
            return y;
        }

        Vector operator+ (const Vector& v) const {
            return Vector(x+v.x, y+v.y);
        }

        Vector operator- (const Vector& v) const {
            return Vector(x-v.x, y-v.y);
        }

        Point operator+ (const Point& p) const {
            return Point(x+p.X(), y+p.Y());
        }

        Vector operator+(void) const {
            return Vector(x, y);
        }

        Vector operator-(void) const {
            return Vector(-x, -y);
        }

        double operator*(const Vector v) const {
            return x*v.x+y*v.y;
        }

        Vector operator*(const double c) const {
            return Vector(c*x, c*y);
        }

        double mod(void) const{
            return sqrt(x*x);
        }

        Vector n (void) const {
            return Vector(-y, x);
        }

        Vector unit (void) const {
            return Vector(x/this->mod(), y/this->mod());
        }

        Vector& norm (void) {
            double len = this->mod();
            x /= len; y /= len;
            return *this;
        }
        
       	Vector operator+ (const Vector& u, const Vector& v) {
    	    return Vector(u.X()+v.X(), u.Y()+v.Y());
		}
	    Vector operator- (const Vector& u, const Vector& v);
	    Point operator+ (const Vector& v, const Point& p);
	    Vector operator+(const Vector& v);
	    Vector operator-(const Vector& v);
	    
	    double operator*(const Vector& u, const Vector v);

	    Vector operator*(const Vector& v, const double c);
	    Point toPoint (const Vector& v);
    };

}

#endif
