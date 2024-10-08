#include <iostream>
#include <cmath>
#include "R3Graph.h"

using namespace std;
using namespace R3Graph;

R3Vector radiusVector(double lat, double lon);

double EARTH_RADIUS = 6378000.0;
const double PI = 3.14159265358979323846;

int main() {
    while (true) {
        double mlat, mlon;        
        double x, y;
        cout << "Enter coordinates of centre of map and point on map:" << endl;
        cin >> mlat >> mlon >> x >> y;
        if (!cin.good())
            break;
        R3Vector rm = radiusVector(mlat, mlon);
	R3Vector x_ = (R3Vector(0.0 , 0.0 , 1.0).vectorProduct(rm)).normalize();
	R3Vector y_ = (rm.vectorProduct(x_)).normalize();
	
	R3Vector base(EARTH_RADIUS*rm+x*x_+y*y_);
        double theta = 90 - (base.angle(R3Vector(0.0, 0.0, 1.0)))*(180.0)/PI;
    	double phi = atan2(base.y, base.x)*(180.0)/PI;
		//(R3Vector(base.x, 0.0, 0.0).angle(R3Vector(base.x, base.y, 0.0)))*(180.0)/PI;

        cout << "lat = " << theta << " lon = " << phi << endl;
    }
    return 0;
}

R3Vector radiusVector(double lat, double lon) {
    double phi = lon*PI/180.;   // Convert to radians
    double theta = lat*PI/180.;
    
    // Rotate ex around ez by angle phi: v0 = (cos(phi), sin(phi), 0.)
    // Rotate v0 in meridional plane by angle theta:
    // v1 = (cos(phi)*cos(theta), sin(phi)*cos(theta), sin(theta))
    double cosTheta = cos(theta);
    return R3Vector(cos(phi)*cosTheta, sin(phi)*cosTheta, sin(theta));
}
