#ifndef R2GEOMETRY
#define R2GEOMETRY

#include <cmath>

#define EPS (1e-15)

namespace R3Geometry {

	class Point;
	class Vector;

	Vector operator+ (const Vector& u, const Vector& v);
	Vector operator- (const Vector& u, const Vector& v);
	Point operator+ (const Vector& v, const Point& p);
	Vector operator+(const Vector& v);
	Vector operator-(const Vector& v);

	double operator*(const Vector& u, const Vector v);

	Vector operator*(const Vector& v, const double c);
	Vector operator*(const double c, const Vector& v);

	Point operator* (const double c, const Point& p);
	Point operator* (const Point& p, const double c);

	Vector RVector (const Point& p);
	
	Point operator+ (const Point& p, const Vector& v);
	Point operator+ (const Point& p, const Point& q);
	Vector operator- (const Point& p, const Point& q);

	Point RPoint (const Vector& v);
	
	class Point {
		double x;
		double y;
		double z;

		public:

		Point(double xx = 0, double yy = 0, double zz = 0) {
			if (fabs(xx) < EPS) x = 0;
			else x = xx;

			if (fabs(yy) < EPS) y = 0;
			else y = yy; 

			if (fabs(zz) < EPS) z = 0;
			else z = zz;
		}
		
		Point(const Point& p) {
			x = p.x;
			y = p.y;
			z = p.z;
		}

		Point(Point&& p) {
			x = p.x; p.x = 0;
			y = p.y; p.y = 0;
			z = p.z; p.z = 0;
		}

		Point& operator= (const Point& p) {
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		}

		Point& operator= (Point&& p) {
			x = p.x; p.x = 0;
			y = p.y; p.y = 0;
			z = p.z; p.z = 0;
			return *this;
		}
	
		double X(void) const {
			return x;
		}

		double Y(void) const {
			return y;
		}

		double Z(void) const {
			return z;
		}
	};

	class Vector {
		double x;
		double y;
		double z;

		public:

		Vector(double xx = 0, double yy = 0, double zz = 0) {
			if (fabs(xx) < EPS) x = 0;
			else x = xx;

			if (fabs(yy) < EPS) y = 0;
			else y = yy;

			if (fabs(zz) < EPS) z = 0;
			else z = zz;
		}

		Vector(const Vector& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector(Vector&& v) {
			x = v.x; v.x = 0;
			y = v.y; v.y = 0;
			z = v.z; v.z = 0;
		}

		Vector& operator= (const Vector& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
	
		Vector& operator= (Vector&& v) {
			x = v.x; v.x = 0;
			y = v.y; v.y = 0;
			z = v.z; v.z = 0;
			return *this;
		}

		double X(void) const{
			return x;
		}

		double Y(void) const{
			return y;
		}

		double Z(void) const{
			return z;
		}

		Vector operator+ (const Vector& v) const {
			return Vector(x+v.x, y+v.y, z+v.z);
		}

		Vector operator- (const Vector& v) const {
			return Vector(x-v.x, y-v.y, z-v.z);
		}

		Point operator+ (const Point& p) const {
			return Point(x+p.X(), y+p.Y(), z+p.Z());
		}

		Vector operator+(void) const {
			return Vector(x, y, z);
		}

		Vector operator-(void) const {
			return Vector(-x, -y, -z);
		}

		double operator*(const Vector v) const {
			return x*v.x+y*v.y+z*v.z;
		}

		Vector operator*(const double c) const {
			return Vector(c*x, c*y, c*z);
		}

		double mod(void) const{
			return sqrt(x*x);
		}

		Vector n (void) const {
			return Vector(-y, x, 0);
		}

		Vector unit (void) const {
			return Vector(x/this->mod(), y/this->mod(), z/this->mod());
		}

		Vector& norm (void) {
			double len = this->mod();
			x /= len; y /= len; z /= len;
			return *this;
		}
	};

}

#endif
