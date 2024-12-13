// класс вершин многоугольников

#pragma once

class vertex {
	public:
		double x, y;
		vertex (double x = 0, double y = 0) : x(x), y(y) {}
		void print_as_pair(); // вывод вершины на экран
}