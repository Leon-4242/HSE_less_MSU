// класс многоугольников

#pragma once

#include "vertex.h"
#include <vector>

class polygon {
	private:
		vector<vertex> vertices; // массив из вершин
	public:
		void create(vector<vertex> newvertices); // создать многоугольник
		void remove(); // удалить многоугольник
		int count_vertices; // посчитать количество вершин многоугольника
		void print_as_list; // вывести многоугольник на экран в виде списка вершин
		bool check_vertex(vertex vert); // проверить вложение vert в данный многоугольник
		bool check_all_vertices(polygon keypol); // проверить вложение всех вершин keypol в данный многоугольник
		bool check_coverage(polygon keypol); // проверить вложение keypol в данный многоугольник
}		