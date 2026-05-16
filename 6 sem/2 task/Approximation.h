#ifndef APPROXIMATION_H
#define APPROXIMATION_H

#include "R2Geometry.h"
#include "R3Geometry.h"
#include "CommonDefs.h"
#include <cassert>
#include <vector>
#include <functional>

#define TIMER (0)

namespace Approx {
	using namespace R3Geometry;

	class Approximator {
		double ax, bx;
		double ay, by;
		int nx, ny;
		double p;

		double (*f)(R2Geometry::Point);

		double pixel;
		R2Geometry::Point mid;

		std::vector<R2Geometry::Point> points;
		std::vector<double> values;
		std::vector<double> coeff;

		void makeApproxPiecePolynom(void);

		std::pair<double, double> minMaxChangeable(double (Approximator::*)(R2Geometry::Point));

		public:
		Approximator(double, double, int, double (*)(R2Geometry::Point) = [](R2Geometry::Point x) {return x.X();});

		double approxPiecePolynom(R2Geometry::Point);
		double errorPiecePolynom(R2Geometry::Point);

		double origin(R2Geometry::Point);

		std::vector<std::pair<double, double>> minMax(void);

		void setFunction(double (*)(R2Geometry::Point));
		void setError(double);
		void setN(int);
		void setPixel(double);
	};

	void solve(std::vector<double> &, std::vector<double> &, std::vector<double> &, std::vector<double> &, int);	
}

#endif //APPROXIMATION_H
