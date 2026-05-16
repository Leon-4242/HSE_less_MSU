#include "R2Geometry.h"

namespace R2Geometry {
	Vector operator+ (const Vector& u, const Vector& v) {
		return Vector(u.X()+v.X(), u.Y()+v.Y(), u.Z()+v.Z());
	}

	Vector operator- (const Vector& u, const Vector& v) {
		return Vector(u.X()-v.X(), u.Y()-v.Y(), u.Z()-v.Z());
	}

	Point operator+ (const Vector& v, const Point& p) {
		return Point(v.X()+p.X(), v.Y()+p.Y(), v.Z()+p.Z());
	}

	Vector operator+(const Vector& v) {
		return Vector(v.X(), v.Y(), v.Z());
	}

	Vector operator-(const Vector& v) {
		return Vector(-v.X(), -v.Y(), -v.Z());
	}

	double operator*(const Vector& u, const Vector v) {
		return u.X()*v.X()+u.Y()*v.Y()+u.Z()*v.Z();
	}

	Vector operator*(const Vector& v, const double c) {
		return Vector(c*v.X(), c*v.Y(), c*v.Z());
	}

	Vector operator*(const double c, const Vector& v) {
		return Vector(c*v.X(), c*v.Y(), c*v.Z());
	}

	Point operator* (const double c, const Point& p) {
		return Point(c*p.X(), c*p.Y(), c*p.Z());
	}

	Point operator* (const Point& p, const double c) {
		return Point(c*p.X(), c*p.Y(), c*p.Z());
	}

	Vector RVector (const Point& p) {
		return Vector(p.X(), p.Y(), p.Z());
	}

	Point operator+ (const Point& p, const Vector& v) {
		return Point(p.X()+v.X(), p.Y()+v.Y(), p.Z()+v.Z());
	}

	Point operator+ (const Point& p, const Point& q) {
		return Point (q.X() + p.X(), q.Y() + p.Y(), q.Z() + p.Z());
	}

	Vector operator- (const Point& p, const Point& q) {
		return Vector(p.X()-q.X(), p.Y()-q.Y(), p.Z()-q.Z());
	}

	Point RPoint (const Vector& v) {
		return Point(v.X(), v.Y(), v.Z());
	}
	
}
