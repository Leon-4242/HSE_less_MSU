#ifndef RENDERER_H
#define RENDERER_H

#include "Geometry.h"
#include <iostream>
#include <cmath>
#include <utility>

struct ProjectedSegment 
{
    R2Geometry::Point a;
    R2Geometry::Point b;
    bool visible;
};

class Camera 
{
	R3Geometry::Point target;
	
	Quaternion orientation;
	double focus;
	double dist;

	double screenHalfHeight;
	double aspect;

	double W, H;

	public:

	Camera(R3Geometry::Point, Quaternion, double, double, double);

	std::pair<R2Geometry::Point, bool> pr(const R3Geometry::Point &) const;
	ProjectedSegment projectSegment(const R3Geometry::Point &, const R3Geometry::Point &) const;

	void orbitYaw(double);
	void orbitPitch(double);

	void moveForward(double);
	void moveRight(double);
	void moveUp(double);
	
	void setDist(double);
	void setAspect(double);

	std::string orien (void) const;
	private:

	R3Geometry::Point position() const;

	R3Geometry::Vector toView(const R3Geometry::Point &) const;
	std::pair<R2Geometry::Point, bool> toProj(const R3Geometry::Vector &) const;

	void orbitGlobal(const R3Geometry::Vector &, double);
	void orbitLocal(const R3Geometry::Vector &, double);

	void move(const R3Geometry::Vector &, double);

	bool clipTest(double, double, double &, double &) const;
};

class Light {
	public:

	using Color = R3Geometry::Point;

	Color color = {255,0,0};
	double intensity = 1.0;
	
	virtual Color compute(const R3Geometry::Point &, const R3Geometry::Vector &) const = 0;
	virtual ~Light() = default;
};

class PointLight : public Light 
{
	public:
	R3Geometry::Point position;

	PointLight (const R3Geometry::Point &position): position(position) {}
	Color compute(const R3Geometry::Point &, const R3Geometry::Vector &) const override;
};


#endif //3DRENDER_H
