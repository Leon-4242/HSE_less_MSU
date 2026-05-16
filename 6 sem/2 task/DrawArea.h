#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QResizeEvent>
#include <QPainterPath>

#include "CommonDefs.h"
#include "Approximation.h"
#include "Data.h"

#include <iostream>
#include <cmath>

class Function {
	public:
		std::string name;
		double (*f)(Point);
		Function(std::string, double (*)(Point));
};

class DrawArea : public QWidget 
{
	Q_OBJECT
private:
	std::vector<Function> functions;

	double ax, bx;
	double ay, by;

	int mx, my;

	QColor bgColor;

	double xMin, xMax, yMin, yMax;
	double xCoeff, yCoeff;

	Data *data;

	Camera camera;

	QPointF mapToPixels(const Approx::Point &) const;
	Approx::Point mapFromPixels(const QPointF &) const;
	QPoint toIntPoint(const QPointF &) const;

	bool pointsDrawFlag;
	Approx::Approximator approximator;
public:
	DrawArea(QWidget *, double, double, double, double, int, int, Data *);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void changeFunc();
	void changeType();
	void scaleUp();
	void scaleDown();
	void numberUp();
	void numberDown();
	void errorUp();
	void errorDown();
	void points();

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private:
	void recomputeBounds();
	
	void drawCoordSystem(QPainter *);
//	void drawPoints(QPainter *);
	
	void drawFunction(QPainter *, double (Approx::Approximator::*)(Point)); 

	void drawOrigin(QPainter *);
	void drawApprox(QPainter *);
	void drawError(QPainter *);
	
	void initializeMap();
};

#endif // DRAWAREA_H
