#include <iostream>
#include "vertex.h"
#include "polygon.h"
#include <vector>
#include <cmath>

vertex vert; // вершина
vert.vertex(double x, double y) : x(x), y(y) {}

polygon pol; // многоугольник

void vert.print_as_pair() { // вывод вершины на экран
	cout << x << ", " << y << endl;
}

void pol.create(vector<vertex> newvertices) { // создать многоугольник
	vertices = newvertices;
}

void pol.remove() { // удалить многоугольник
	vertices.clear();
} 

int pol.count_vertices() { // посчитать количество вершин pol
	return vertices.size();
}

void pol.print_as_list(pol) { // вывести pol на экран в виде списка вершин
	if (vertices.empty == FALSE) {
		int n = pol.count_vertices;
		for (const auto& vert : vertices {
			vert.print_as_pair()
		}
	}
	else {
		cout << "polygon is blank" << endl;
	}
}
	
bool check_coverage(keypol, pol) // проверить вложение keypol в pol (метод суммирования углов)
	int k = keypol.count_vertices;
	int n = pol.count_vertices;
	double angle_summ = 0.0;
	for (int i = 0; i < k; i++) {
		vertex vert = {keypol[i].x, keypol[i].y}
		for (int j = 0; j < n; j++):
			vertex a = {pol[j].x - vert.x, pol[j].y - vert.y};
			vertex b = {pol[(j + 1) % n].x - vert.x, pol[pol[(j + 1) % n].y - vert.y};
			double prod1 = a.x * b.y - a.y * b.x;
			double prod2 = a.x * b.x + a.y * b.y;
            double angle = atan2(prod1, prod2);
            angle_summ += angle;
		if (fabs(angle_summ) < 1e-9) == FALSE {
			return FALSE;
		}
	}
	return TRUE;
}
