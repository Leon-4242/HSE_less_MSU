#include <vector>
#include <iostream>
#include <fstream>
#include "lib.hpp"

int main() {
	std::vector<point> points;
    int answer;
	std::cout << "Work with file?\n1)Yes\n2)No\n";
	std::cin >> answer;

    if (answer == 2) {
        int n;
		std::cout << "Enter number of points: ";
		std::cin >> n;
        points.resize(n);
        for (int i = 0; i < n; i++) {
			std::cin >> points[i].x >> points[i].y;
        }
    } else if (answer == 1){
		std::string filename;
		std::cout << "Enter filename: ";
		std::cin >> filename;
		std::ifstream f(filename);
        if (!f) {
			std::cerr << "File not found.\n";
            return 1;
        }
        point p;
        while (f >> p.x >> p.y) {
            points.push_back(p);
        }
    }
	else 
	{
		return 0;
	}

    circle C = minimal_enclosing_circle(points);

	std::cout << "Center: (" << C.c.x << ", " << C.c.y << ")\n";
	std::cout << "Radius: " << C.r << "\n";

    return 0;
}

