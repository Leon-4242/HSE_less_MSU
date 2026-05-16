#include "DrawArea.h"

Function::Function (std::string name, double(*f)(Point)): name(name), f(f) 
{}

DrawArea::DrawArea (QWidget *parent, double ax, double bx, double ay, double by, int nx, int ny, int mx, int my, int k, Data *data) :
	QWidget(parent),
	ax(ax), bx(bx),
	ay(ay), by(by),
	mx(mx), my(my),
	bgColor(Qt::lightGray),
	xMin(ax - 0.01*(bx-ax)),
	xMax(bx + 0.01*(bx-ax)),
	yMin(ay - 0.01*(by-ay)),
	yMax(by + 0.01*(by-ay)),
	xCoeff(50.),		
	yCoeff(50.),		
	data(data),
	pointsDrawFlag(true),
	approximator(ax, bx, ay, by, nx, ny)
{
	setAttribute(Qt::WA_StaticContents); // for optimizing painting events

	functions = std::vector<Function>
	{
		Function("f(x, y) = 1", [](Point x) {(void)x; return 1.0;}),
		Function("f(x, y) = x", [](Point x) {return x.X();}),
		Function("f(x, y) = y", [](Point x) {return x.Y();}),
		Function("f(x, y) = x+y", [](Point x) {return x.X()+x.Y();}),
		Function("f(x, y) = (x^2+y^2)^(1/2)", [](Point x) {return sqrt(x.X()*x.X() + x.Y()*x.Y());}),
		Function("f(x, y) = x^2+y^2", [](Point x) {return x.X()*x.X() + x.Y()*x.Y();}),
		Function("f(x, y) = e^(x^2-y^2)", [](Point x) {return exp(x.X()*x.X() - x.Y()*x.Y());}),
		Function("f(x, y) = 1/(25(x^2+y^2)+1)", [](Point x) {return 1/(25*(x.X()*x.X()+x.Y()*x.Y())+1);})
	};
	
	data->k = k;
	data->name = functions[k].name;

	data->origin = true;
	data->piecePolynom = false;
	data->error = false;
	data->s = 0;
	data->nx = nx;
	data->ny = ny;
	data->p = 0;

	approximator.setFunction(functions[data->k].f);
}

void DrawArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, false);
	recomputeBounds(); 
	initializeMap();
	drawCoordSystem(&painter);

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	painter.setRenderHint(QPainter::Antialiasing, true);
//	if (pointsDrawFlag)
//		drawPoints(&painter);
	
	if (data->origin)
		drawOrigin(&painter);
	if (data->piecePolynom)
		drawApprox(&painter);
	if (data->error)
		drawError(&painter);

	event->accept();
}


void DrawArea::drawCoordSystem(QPainter *painter) 
{
	int w = width();
	int h = height();
	if (w == 0 || h == 0)
		return;

	painter->setBrush(QBrush(bgColor));
	painter->drawRect(0, 0, w, h);
	painter->setBrush(Qt::NoBrush);

	QPen penBlack(Qt::black);
	penBlack.setWidth(2);
	painter->setPen(penBlack);
	
	if (yMin < 0.0 && 0.0 < yMax) {
		painter->drawLine(mapToPixels(Approx::Point(xMin, 0)), mapToPixels(Approx::Point(xMax, 0)));
	}
	if (xMin < 0.0 && 0.0 < xMax) {
		painter->drawLine(mapToPixels(Approx::Point(0, yMin)), mapToPixels(Approx::Point(0, yMax)));
	}

	QPointF center = mapToPixels(Approx::Point(0, 0));
	
	QPointF dx(8., 0.);
	QPointF dy(0., 8.);

	QPointF xTick(100., 0);
	QPointF yTick(0, 100.);

	painter->drawText(center.x()+10, center.y()-10, QString("0"));
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

}

/*
void DrawArea::drawPoints(QPainter *painter) 
{
	QPen bluePen(Qt::blue);
	bluePen.setWidth(2);

	painter->setPen(bluePen);

	QPointF dx(8., 0.);
	QPointF dy(0., 8.);
	
	int counter  = 0;
	double x = std::max(xMin, ax);
	double y = std::max(yMin, ay);
	double z = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	
	while (x - std::min(xMax, b) < EPSILON_FOR_COMPARE) {
		QPointF t = mapToPixels(Approx::Point(x, y));
		painter->drawLine(t-dx, t+dx);
		painter->drawLine(t-dy, t+dy);

		++counter;
		x += (b-a)/(data->n-1);
		y = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	}
}
*/

void DrawArea::drawFunction(QPainter *painter, double (Approx::Approximator::*function)(double)) 
{
	QPainterPath path;
	
	Approx::Point h0 = mapFromPixels(QPointF(0., 0., 0.));
	Approx::Point h1 = mapFromPixels(QPointF(1., 1., 0.));
    
	double xDelta = h1.X() - h0.X();
	double yDelta = h1.Y() - h0.Y(); 

	double x = std::max(xMin, ax);
	double y = std::max(yMin, ay);
	double z = 

	path.moveTo(mapToPixels(Approx::Point(x, y)));

	while (x-std::min(xMax, b) < EPSILON_FOR_COMPARE) {
		x += xDelta;
		y = (approximator.*function)(x);
		path.lineTo(mapToPixels(Approx::Point(x, y)));
	}

	painter->drawPath(path);
}

void DrawArea::drawOrigin(QPainter *painter) 
{
	QPen pen(Qt::red);
	pen.setWidth(1);
	painter->setPen(pen);
	
	drawFunction(painter, &Approx::Approximator::origin);
} 


void DrawArea::drawApprox(QPainter *painter) 
{
	QPen pen(Qt::darkGreen);
	pen.setWidth(1);
	painter->setPen(pen);

	drawFunction(painter, &Approx::Approximator::approxPiecePolynom);
}

void DrawArea::drawError(QPainter *painter) 
{
	QPen pen(Qt::darkRed);
	pen.setWidth(1);
	painter->setPen(pen);
	
	drawFunction(painter, &Approx::Approximator::errorPiecePolynom);
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
	int w = width();
	int h = height();
	if (w == 0 || h == 0)
		return;
	
	double dx = xMax - xMin;
	double dy = yMax - yMin;

	if (fabs(dx) < EPSILON_FOR_COMPARE || fabs(dy) < EPSILON_FOR_COMPARE)
		return;	
	
	xCoeff = double(w) / dx;
	yCoeff = double(h) / dy;
}

void DrawArea::recomputeBounds()
{
	std::vector<std::pair<double, double>> minMax = approximator.minMax();

	data->originAbsMax = std::max(fabs(minMax[0].first), fabs(minMax[0].second));
	data->piecePolynomAbsMax = std::max(fabs(minMax[1].first), fabs(minMax[1].second));
	data->errorPiecePolynomAbsMax = std::max(fabs(minMax[2].first), fabs(minMax[2].second));

	double max, min;
	if (data->origin) {
		max = minMax[0].second;
		min = minMax[0].first;
	} else if (data->piecePolynom) {
		max = minMax[1].second;
		min = minMax[1].first;
	} else {
		max = minMax[2].second;
		min = minMax[2].first;
	}

	double zDelta = (max-min < 0.001 ? 1.5 : 0.01*(max-min));
	zMin = min - zDelta;
	zMax = max + zDelta;

	emit data->changed();
}



void DrawArea::changeFunc()
{
	data->k = (data->k+1)%functions.size();	
	data->name = functions[data->k].name;
	data->p = 0;

	approximator.setFunction(functions[data->k].f);

	emit data->changed();
	update();
}

void DrawArea::changeType()
{
	if (data->origin) {
		data->origin = false;
		data->piecePolynom = true;
	} else if (data->piecePolynom) {
		data->piecePolynom = false;
		data->error = true;
	} else {
		data->origin = true;
		data->error = false;
	}

	emit data->changed();

	update();
}

void DrawArea::scaleUp()
{
	if (xMax - xMin < 1e7) {
		double buff = (3.*xMin-xMax)/2.;
		xMax = (3.*xMax-xMin)/2.;
		xMin = buff;

		++(data->s);
	}

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	emit data->changed();

	update();
}

void DrawArea::scaleDown()
{
	if (xMax - xMin > 1e-4) {
		double buff = (3.*xMin+xMax)/4.;
		xMax = (3.*xMax+xMin)/4.;
		xMin = buff;

		--(data->s);
	}

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	emit data->changed();

	update();
}

void DrawArea::numberUp()
{
	if (data->n < 1e7) {
		data->n *= 2;
		approximator.setN(data->n);
	}

	emit data->changed();

	update();
}

void DrawArea::numberDown()
{
	if (data->n >= 10) {
		data->n /= 2;
		approximator.setN(data->n);
	}

	emit data->changed();

	update();
}

void DrawArea::errorUp()
{
	if (data->p < 10) {
		++(data->p);
		approximator.setError(data->p*0.1*data->originAbsMax);
	}

	emit data->changed();

	update();
}

void DrawArea::errorDown()
{
	if (data->p > -10) {
		--(data->p);
		approximator.setError(data->p*0.1*data->originAbsMax);
	}

	emit data->changed();

	update();
}

void DrawArea::points()
{
	pointsDrawFlag = !pointsDrawFlag;
	update();
}



QPointF DrawArea::mapToPixels(const Approx::Point &p) const 
{
	return QPointF((p.X() - xMin)*xCoeff, (yMax - p.Y())*yCoeff);
}

QPointF DrawArea::mapToPixels(const Approx::Point &p) const 
{
	R2Geometry::Point point = camera.pr(p);
	return QPointF((0.5 + point.X()/xCoeff)*width(), (0.5 - point.Y()/yCoeff)*height());
}


Approx::Point DrawArea::mapFromPixels(const QPointF &p) const 
{
	return Approx::Point(xMin + (p.x()/xCoeff), yMax - (p.y()/yCoeff));
}

QPoint DrawArea::toIntPoint(const QPointF &p) const 
{
	return QPoint(std::round(p.x()), std::round(p.y()));
}

QSize DrawArea::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize DrawArea::sizeHint () const
{
  return QSize (1000, 1000);
}
