// Print a distance in meters between 2 points on the Earth surface.
// Coordinates of points are given as latitude and longtitude in degrees.
//
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
        double lat0, lon0;        
        double lat1, lon1;
        cout << "Enter coordinates of 2 points on the Earth surface:" << endl;
        cin >> lat0 >> lon0 >> lat1 >> lon1;
        if (!cin.good())
            break;
        R3Vector v0 = radiusVector(lat0, lon0);
        R3Vector v1 = radiusVector(lat1, lon1);
        double alpha = v0.angle(v1);    // atan2(y, x)
        double dist = alpha*EARTH_RADIUS;
        cout << "Distance = " << dist <<endl;
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
