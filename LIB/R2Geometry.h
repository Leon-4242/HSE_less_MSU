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
    template <typename T>
    class Point;
    template <typename T>
    class Vector;

    Vector operator*(const double c, const Vector& v);

    Point operator* (const double c, const Point& p);

    class Point {
        double x;
        double y;
    
        public:
    
        Point(double xx = 0, double yy = 0) {
            if (fabs(xx) < EPS) x = 0;
            else x = xx;

            if (fabs(yy) < EPS) y = 0;
            else y = yy; 
        }
        
        Point(const Point& p) {
            x = p.x;
            y = p.y;
        }

        Point(Point&& p) {
            x = p.x; p.x = 0;
            y = p.y; p.y = 0;
        }

        Point& operator= (const Point& p) {
            x = p.x;
            y = p.y;
            return *this;
        }

        Point& operator= (Point&& p) {
            x = p.x; p.x = 0;
            y = p.y; p.y = 0;
            return *this;
        }
    
        double X(void) const {
            return x;
        }

        double Y(void) const {
            return y;
        }
        
        Point operator+ (const Point& p, const Vector& v);
    	Point operator+ (const Point& p, const Point& q);
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
