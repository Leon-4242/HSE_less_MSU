#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QResizeEvent>
#include <QPolygonF>

#include "CommonDefs.h"

#include <iostream>
#include <cmath>
#include <unordered_set>

#include "3DRender.h"
#include "Geometry.h"

struct Segment {
    QPointF a;
    QPointF b;
    bool visible;
	QRgb color;
};

class DrawArea : public QWidget 
{
	Q_OBJECT
private:
	using Triangle = struct
	{
		int a;
		int b;
		int c;
		double depth;
	};
	int n, m;

	std::vector<R3Geometry::Point> points;
	std::vector<Triangle> triangles;
	double ax, bx;
	double ay, by;

	QColor bgColor;

	int w, h;
	double dist;

	R3Geometry::Point center;
	Camera camera;

	R3Geometry::Point lightPos;
	PointLight light;


	bool rotating = false;
	QPoint lastMousePos;

	std::pair<QPointF, bool> mapToPixels(const R3Geometry::Point &) const;
	Segment segToPixels(const R3Geometry::Point &, const R3Geometry::Point &) const;
public:
	DrawArea(QWidget *, double, double, double, double);

	void scaleUp();
	void scaleDown();

	void rotate();
	void orbit();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private:
	void drawCoordSystem(QPainter *);
	void drawRectangle(QPainter *);

	void drawMesh(QPainter *, const std::vector<Triangle> &, const std::vector<R3Geometry::Point> &);

	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

	void wheelEvent(QWheelEvent *);

	void initializeMap();

	QColor converstion(const PointLight::Color &) const;
	QRgb toRgb(const PointLight::Color &) const;
//	void render(QPainter *, std::vector<R3Geometry::Point)	
};

#endif // DRAWAREA_H
