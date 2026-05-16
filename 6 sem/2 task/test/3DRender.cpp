#include "3DRender.h"

Camera::Camera(R3Geometry::Point target, Quaternion orien, double f, double d, double asp): 
	target(target), orientation(orien), focus(f), dist(d),
	screenHalfHeight(1/sqrt(3)), aspect(asp)
{
	orientation.normalize();
	W = focus/screenHalfHeight;
	H = focus/(screenHalfHeight*aspect);
}

R3Geometry::Vector Camera::toView(const R3Geometry::Point &x) const 
{
	return orientation.conjugate().rotate(x - position());
}

std::pair<R2Geometry::Point, bool> Camera::toProj(const R3Geometry::Vector &v) const 
{
	if (v.Z() > -1e-6)
		return {R2Geometry::Point(0, 0), false};

	double div = 1/v.Z(); 
	double x = (-W) * v.X()*div;
	double y = (-H) * v.Y()*div;
	return {R2Geometry::Point(x, y), fabs(x) <= 0.5 && fabs(y) <= 0.5};
}

std::pair<R2Geometry::Point, bool> Camera::pr(const R3Geometry::Point &x) const 
{
	return toProj(toView(x));
}

ProjectedSegment Camera::projectSegment(const R3Geometry::Point &a, const R3Geometry::Point &b) const
{
	auto va = toView(a);
	auto vb = toView(b);

	constexpr double near = 0.01;

	ProjectedSegment res{R2Geometry::Point(0, 0), R2Geometry::Point(0, 0), false};
	if (va.Z() >= -near && vb.Z() >= -near)
		return res;

	if (va.Z() < -near && vb.Z() >= -near) {
		double t = (-near - va.Z())/(vb.Z() - va.Z());
		vb = va + t*(vb-va);
	}

	if (va.Z() >= -near && vb.Z() < -near) {
		double t = (-near - va.Z())/(vb.Z() - va.Z());
		va = va + t*(vb-va);
	}

	auto pa = toProj(va).first;
	auto pb = toProj(vb).first;

	double dx = pb.X() - pa.X();
	double dy = pb.Y() - pa.Y();

	double t0 = 0.0;
	double t1 = 1.0;

	if (
		clipTest(-dx, pa.X() + 0.5, t0, t1) &&
		clipTest( dx, 0.5 - pa.X(), t0, t1) &&
		clipTest(-dy, pa.Y() + 0.5, t0, t1) &&
		clipTest( dy, 0.5 - pa.Y(), t0, t1)
	)
	{
		res.a = pa + t0*(pb-pa);
		res.b = pa + t1*(pb-pa);
		res.visible = true;
	}

	return res;
}

bool Camera::clipTest(double p, double q, double &t0, double &t1) const
{
	if (std::abs(p) < 1e-12)
		return q >= 0;

	double r = q / p;

	if (p < 0) {
		if (r > t1)
			return false;
		if (r > t0)
			t0 = r;
	} else {
		if (r < t0)
			return false;
		if (r < t1)
			t1 = r;
	}

	return true;
}

void Camera::orbitYaw(double angle)
{
	orbitGlobal(R3Geometry::Vector(0,0,-1), angle);
}

void Camera::orbitPitch(double angle)
{
	orbitLocal(R3Geometry::Vector(1,0,0), angle);
}

void Camera::moveForward(double dist)
{
	move({0, 0, -1}, dist);
}

void Camera::moveRight(double dist)
{
	move({1, 0, 0}, dist);
}

void Camera::moveUp(double dist)
{
	move({0, 1, 0}, dist);
}



void Camera::move(const R3Geometry::Vector &localDir, double dist)
{
	target += dist * orientation.rotate(localDir.unit());
}

void Camera::orbitGlobal(const R3Geometry::Vector &axis, double phi)
{
	Quaternion q(axis.unit(), phi);
	
	orientation = q * orientation;
	orientation.normalize();
}

void Camera::orbitLocal(const R3Geometry::Vector &axis, double phi)
{
	Quaternion q(axis.unit(), phi);
	
	orientation = orientation * q;
	orientation.normalize();
}


void Camera::setDist(double d) 
{
	dist = d;
}

void Camera::setAspect(double asp)
{
	aspect = asp;
	H = focus/(screenHalfHeight*aspect);
}

R3Geometry::Point Camera::position(void) const
{
	return target +  dist*orientation.rotate({0,0,1});
}

std::string Camera::orien (void) const 
{
	return 	"(" + std::to_string(orientation.w) + ", "+ std::to_string(orientation.x) + ", "+ std::to_string(orientation.y) + ", "+ std::to_string(orientation.z) + ")\n";
}


PointLight::Color PointLight::compute(const R3Geometry::Point &point, const R3Geometry::Vector &normal) const
{
	R3Geometry::Vector L((point-position).unit());
	return intensity * std::max(0.0, normal.unit()*L) * color;
}
