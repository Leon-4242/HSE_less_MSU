#include "Approximation.h"
#include <sys/time.h>
#include <iostream>

namespace Approx {

	Approximator::Approximator(double ax, double bx, double ay, double by, int nx, int ny, double (*f)(R2Geometry::Point) ): 
		ax(ax), bx(bx), 
		ay(ay), by(by),
		nx(nx), ny(ny), 
		p(0), f(f), 
		pixel(0), mid(R2Geometry::Point(ax+i*(bx-ax)/(nx-1), ay+j*(by-ay)/(ny-1))),
		points(std::vector<R2Geometry::Point>(nx*ny)), values(std::vector<double>(nx*ny)),
		coeff(std::vector<double>(1))
	{
		for (int i = 0; i < nx; ++i) {
			for (int j = 0; j < ny; ++j) {
				points[i*ny+j] = R2Geometry::Point(ax+i*(bx-ax)/(nx-1), ay+j*(by-ay)/(ny-1)); 
				values[i*ny+j] = f(points[i*ny+j]) + (i == nx/2 && j == ny/2? p : 0);
			}
		}

		makeApproxPiecePolynom();
	}

	void Approximator::makeApproxPiecePolynom(void)
	{
		
	#if TIMER==1	
		struct timeval start, end;
		long long start_us, end_us;

		gettimeofday(&start, NULL);
	#endif

	#if TIMER==1
		gettimeofday(&end, NULL);
 
		start_us = start.tv_sec * 1000000 + start.tv_usec;
		end_us = end.tv_sec * 1000000 + end.tv_usec;
 
		std::cerr <<"\n\n\n\n\n\n\n\n\n\n" << (double)(end_us - start_us) / 1000000. << "\n\n\n\n\n\n\n\n\n\n"<< std::endl;
	#endif

		return;
	}

	double Approximator::approxPiecePolynom(R2Geometry::Point x)
	{
		return 0;
	}

	double Approximator::errorPiecePolynom(R2Geometry::Point x)
	{
		return fabs(f(x)-approxPiecePolynom(x));
	}

	void Approximator::setFunction(double (*func)(R2Geometry::Point))
	{
		f = func;
		p = 0;

		for (int i = 0; i < nx; ++i) {
			for (int j = 0; j < ny; ++j) {
				values[i*ny+j] = f(points[i*ny+j]) + (i == nx/2 && j == ny/2? p : 0);
			}
		}

		makeApproxPiecePolynom();
	}

	void Approximator::setError(double pp)
	{
		p = pp;

		values[nx/2 * ny + ny/2] = f(points[nx/2 * ny + ny/2]) + p;

		makeApproxPiecePolynom();
	}

	
	void Approximator::setN(int nxx, int nyy)
	{
		nx = nxx;
		ny = nyy;
	
		points.resize(nx*ny);
		values.resize(nx*ny);

		coeff.resize(1);

		for (int i = 0; i < nx; ++i) {
			for (int j = 0; j < ny; ++j) {
				points[i*ny+j] = R2Geometry::Point(ax+i*(bx-ax)/(nx-1), ay+j*(by-ay)/(ny-1)); 
				values[i*ny+j] = f(points[i*ny+j]) + (i == nx/2 && j == ny/2? p : 0);
			}
		}

		mid = R2Geometry::Point(ax+i*(bx-ax)/(nx-1), ay+j*(by-ay)/(ny-1));

		makeApproxPiecePolynom();
	}

	void Approximator::setPixel(double Pixel) 
	{
		pixel = Pixel;
	}

	double Approximator::origin(R2Geometry::Point x) 
	{
		return f(x) + (fabs((x-mid).mod())  < 2*pixel ? p : 0);
	}
	
	std::pair<double, double> Approximator::minMaxChangeable(double (Approximator::*ff)(R2Geometry::Point))
	{	
		std::pair<double, double> minMax;
		minMax.first = (this->*ff)({ax, ay});
		minMax.second = (this->*ff)({ax, ay});
		double xDelta = 0.01;
		double yDelta = 0.01;
		for (double x = ax; x - bx < EPSILON_FOR_COMPARE; x += xDelta)
		{
			for (double y = ay; y - by < EPSILON_FOR_COMPARE; y += yDelta) {
				double z = (this->*ff)({x, y});
				if (z < minMax.first)
					minMax.first = z;
				if (z > minMax.second)
					minMax.second = z;
			}
		}
		return minMax;
	}
	std::vector<std::pair<double, double>> Approximator::minMax(void)
	{	
		std::vector<std::pair<double, double>> minMax{};
		minMax.push_back(minMaxChangeable(&Approximator::origin));
		minMax.push_back(minMaxChangeable(&Approximator::approxPiecePolynom));
		minMax.push_back(minMaxChangeable(&Approximator::errorPiecePolynom));
		return minMax;
	}

	void solve(std::vector<double> &dUp, std::vector<double> &d, std::vector<double> &dDown, std::vector<double> &b, int n)
	{
		
		double div;
		for (int i = 0; i < n-1; ++i) {
			div = 1/d[i];
			b[i] *= div;
			dUp[i] *= div;

			b[i+1] -= b[i]*dDown[i];
			d[i+1] -= dUp[i]*dDown[i];
		}

		b[n-1] /= d[n-1];

		for (int i = n-1; i > 0; --i)
			b[i-1] -= b[i]*dUp[i-1];
	}
}
