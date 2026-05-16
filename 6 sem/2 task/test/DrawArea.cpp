#include "DrawArea.h"

DrawArea::DrawArea (QWidget *parent, double ax, double bx, double ay, double by) :
	QWidget(parent),
	n(128), m(128),
	points(std::vector<R3Geometry::Point>(n*m)),
	triangles(std::vector<Triangle>((n-1)*(m-1)*2)),
	ax(ax), bx(bx),
	ay(ay), by(by),
	bgColor(Qt::lightGray),
	dist(2*sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay) + 4)),
	center((ax+bx)/2, (ay+by)/2, 0),
	camera(center, Quaternion(1, 0, 0, 0), 1, 2*(sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay) + 4)), 2.0),
	lightPos({0, 10, 10}),
	light({0, 10, 10})
{
	setAttribute(Qt::WA_StaticContents); // for optimizing painting events
	w = width();
	h = height();
	camera.setAspect(w * (1.0)/h);
	camera.orbitPitch(-3*M_PI/4);
	camera.orbitYaw(3*M_PI/4);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			points[i*m + j] = R3Geometry::Point(ax+i*(bx-ax)/(n-1), ay+j*(by-ay)/(m-1), 0);
		}
	}

	for (int i = 0; i < n-1; ++i) {
		for (int j = 0; j < m-1; ++j) {
			triangles[2*(i*(m-1) + j)].a = i*m+j;
			triangles[2*(i*(m-1) + j)].b = i*m+j+1;
			triangles[2*(i*(m-1) + j)].c = (i+1)*m+j;

			triangles[2*(i*(m-1) + j)+1].a = i*m+j+1;;
			triangles[2*(i*(m-1) + j)+1].b = (i+1)*m+j+1;
			triangles[2*(i*(m-1) + j)+1].c = (i+1)*m+j;

		}
	}

}

void DrawArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);
	initializeMap();
	drawCoordSystem(&painter);

	drawRectangle(&painter);

	painter.drawText(20, 20, QString::fromStdString(camera.orien()));	
	event->accept();
}


void DrawArea::drawCoordSystem(QPainter *painter) 
{
	painter->setBrush(QBrush(bgColor));
	painter->drawRect(0, 0, w, h);
	painter->setBrush(Qt::NoBrush);

	double L = 1e6*dist;

	QPen penBlack(Qt::black);
	penBlack.setWidth(2);
	painter->setPen(penBlack);
	
	auto OX = segToPixels({-L, 0, 0}, {L, 0, 0});
	if (OX.visible)
		painter->drawLine(OX.a, OX.b);

	auto OY = segToPixels({0, -L, 0}, {0, L, 0});
	if (OY.visible)
		painter->drawLine(OY.a, OY.b);

	auto OZ = segToPixels({0, 0, -L}, {0, 0, L});
	if (OZ.visible)
		painter->drawLine(OZ.a, OZ.b);

	auto zero = mapToPixels({0, 0, 0});
	if (zero.second)
		painter->drawText(zero.first.x()+10, zero.first.y()-10, QString("0"));

	if (OX.visible)
		painter->drawText(OX.b+QPointF((w/2 - OX.b.x() < 0 ? -1 : 1)*15, -10), QString("X"));

	if (OY.visible)
		painter->drawText(OY.a+QPointF((w/2 - OY.a.x() < 0 ? -1 : 1)*15, -10), QString("Y"));

	if (OZ.visible)
		painter->drawText(OZ.a+QPointF(10, (h/2 - OZ.a.y() < 0 ? -1 : 1)*15), QString("Z"));
	/*

	QPointF center = mapToPixels(Approx::Point(0, 0));
	
	QPointF dx(8., 0.);
	QPointF dy(0., 8.);

	QPointF xTick(100., 0);
	QPointF yTick(0, 100.);

	int i = 0;
	while ((center+(++i)*xTick).x() < w) {
		painter->drawLine((center+i*xTick)-dy, (center+i*xTick)+dy);
		painter->drawText((center+i*xTick).x(), (center+i*xTick).y() - 20, QString::fromStdString(std::to_string(mapFromPixels(center+i*xTick).X())));
	}

	i = 0;
	while ((center+(--i)*xTick).x() > 0) {
		painter->drawLine((center+i*xTick)-dy, (center+i*xTick)+dy);
		painter->drawText((center+i*xTick).x(), (center+i*xTick).y() - 20, QString::fromStdString(std::to_string(mapFromPixels(center+i*xTick).X())));
	}

	i = 0;
	while ((center+(++i)*yTick).y() < h) {
		painter->drawLine((center+i*yTick)-dx, (center+i*yTick)+dx);
		painter->drawText((center+i*yTick).x() + 20, (center+i*yTick).y(), QString::fromStdString(std::to_string(mapFromPixels(center+i*yTick).Y())));
	}

	i = 0;
	while ((center+(--i)*yTick).y() > 0) {
		painter->drawLine((center+i*yTick)-dx, (center+i*yTick)+dx);
		painter->drawText((center+i*yTick).x() + 20, (center+i*yTick).y(), QString::fromStdString(std::to_string(mapFromPixels(center+i*yTick).Y())));
	}
	*/
}

void DrawArea::drawRectangle(QPainter *painter) 
{
	drawMesh(painter, triangles, points);
}

//void DrawArea::render(QPainter *painter, std::vector<)

void DrawArea::drawMesh(QPainter *painter, const std::vector<Triangle> &triangles, const std::vector<R3Geometry::Point> &points)
{
	/*
	for (auto tr : triangles) {
		tr.color = toRgb(light.compute(1./3*(points[tr.a]+points[tr.b]+points[tr.c]), (points[tr.b]-points[tr.a]).vect(points[tr.c]-points[tr.a])));
	}

	QImage frameBuffer(width(), height(), QImage::Format_RGB32);

	uchar* bits = frameBuffer.bits();
	int bytesPerLine = frameBuffer.bytesPerLine();	
	*/
	
	PointLight::Color color;
	std::pair<QPointF, bool> a, b, c;

	QPen pen(Qt::NoPen);
	painter->setPen(pen);

	QColor oldColor, newColor;
	for (auto tr : triangles) {
		color = light.compute(1./3*(points[tr.a]+points[tr.b]+points[tr.c]), (points[tr.b]-points[tr.a]).vect(points[tr.c]-points[tr.a]));
		a = mapToPixels(points[tr.a]);
		b = mapToPixels(points[tr.b]);
		c = mapToPixels(points[tr.c]);

		if (!a.second || !b.second || !c.second) 
			return;
		
		newColor = converstion(color);
		if (newColor != oldColor)
			painter->setBrush(QBrush(converstion(color)));
		painter->drawPolygon(QPolygonF() << a.first << b.first << c.first);
		oldColor = newColor;
	}
}

void DrawArea::resizeEvent(QResizeEvent *event) 
{
	if (width() == 0 || height() == 0)
		return;
		
	initializeMap();
	update();	
	event->accept();
}

void DrawArea::initializeMap() 
{
	w = width();
	h = height();
	camera.setAspect(w *(1.)/h);
}

void DrawArea::scaleUp()
{
	dist *= 2;
	camera.setDist(dist);

	update();
}

void DrawArea::scaleDown()
{
	dist /= 2;
	camera.setDist(dist);

	update();
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		rotating = true;
		lastMousePos = event->pos();
	}
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		rotating = false;
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
	if (!rotating)
		return;

	QPoint delta = event->pos() - lastMousePos;
	lastMousePos = event->pos();

	double sensitivity = 0.005;

	camera.orbitYaw(-delta.x() * sensitivity);
	camera.orbitPitch(-delta.y() * sensitivity);
	update();
}

void DrawArea::wheelEvent(QWheelEvent *event)
{
	double numDegrees = event->angleDelta().y() / 120.0;

	dist *= std::pow(0.9, numDegrees);
	camera.setDist(dist);

	update();
}

std::pair<QPointF, bool> DrawArea::mapToPixels(const R3Geometry::Point &p) const 
{
	std::pair<R2Geometry::Point, bool> point = camera.pr(p);
	return {QPointF((0.5 + point.first.X())*w, (0.5 - point.first.Y())*h), point.second};
}

Segment DrawArea::segToPixels(const R3Geometry::Point &p, const R3Geometry::Point &q) const
{
	auto seg = camera.projectSegment(p, q);
	return Segment{QPointF((0.5+seg.a.X())*w, (0.5-seg.a.Y())*h), QPointF((0.5+seg.b.X())*w, (0.5-seg.b.Y())*h), seg.visible, 0};
}

QColor DrawArea::converstion(const PointLight::Color &color) const
{
	return QColor(std::round(color.X()), std::round(color.Y()), std::round(color.Z()));
}

QRgb DrawArea::toRgb(const PointLight::Color &color) const
{
	return ((static_cast<uint32_t>(color.X()) << 16) | (static_cast<uint32_t>(color.Y()) << 8) | static_cast<uint32_t>(color.Z()));
}


QSize DrawArea::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize DrawArea::sizeHint () const
{
  return QSize (1000, 1000);
}
