#include "library_graph.h"
#include <iostream>

using namespace std;

int Graph::num_graph(void) {
	return v_n;
}

void Graph::addVertex(void) {
	v_n++;
	if (v_n == 1) {
		arr = new int[1];
		arr[0] = -1;
		return;
	}
	int *temp = new int[v_n*v_n];
	for (int i = 0; i < v_n; i++) {
		for (int j = 0; j < v_n; j++) {
			if (i == v_n-1 || j == v_n-1) {
				temp[i*v_n+j] = -1;
			} else {
				temp[i*v_n+j] = arr[i*(v_n-1)+j];
			}
		}
	}

	delete[] arr;
	arr = temp;
	temp = nullptr;
}

void Graph::addEdge(int k, int t) {
	if (!this->areAdjacent(k, t) && k > 0 && k <= v_n && t > 0 && t <= v_n && k != t) {
		arr[(k-1)*v_n+t-1] = arr[(t-1)*v_n+k-1] = 0;
	}
}

void Graph::delVertex(int k) {
	if (k <= 0 || k > v_n) {
		return;
	}

	v_n--;
	if (v_n == 0) {
		delete[] arr;
		arr = nullptr;
		return;
	}	
	int flag = 0;
	int *temp = new int[v_n*v_n];
	for (int i = 0; i < v_n; i++) {
		for (int j = 0; j < v_n; j++) {
			flag += i < k-1 ? 10 : 20;
			flag += j < k-1 ? 1 : 2;
			switch (flag) {
				case 11:
					temp[i*v_n+j] = arr[i*(v_n+1)+j];
					break;
				case 12:
                                        temp[i*v_n+j] = arr[i*(v_n+1)+j+1];
                                        break;
				case 21:
                                        temp[i*v_n+j] = arr[(i+1)*(v_n+1)+j];
                                        break;
				case 22:
                                        temp[i*v_n+j] = arr[(i+1)*(v_n+1)+j+1];
                                        break;
			}
			flag = 0;
		}
	}

	delete[] arr;
	arr = temp;
	temp = nullptr;
}
void Graph::delEdge(int k, int t) {
	if (k > 0 && k <= v_n && t > 0 && t <= v_n) {
		arr[(k-1)*v_n+t-1] = arr[(t-1)*v_n+k-1] = -1;
	}
}

void Graph::print(void) {
	std::cout << "\n" << std::endl;
	for (int i = 0; i < this->v_n; i++) {
		for (int j = 0; j < this->v_n; j++) {
			std::cout << arr[i*(this->v_n)+j] << " ";
		}
		std::cout << "\n" << std::endl;
	}
}

int Graph::Weight(int k, int t) {
	if (this->areAdjacent(k, t) && k > 0 && k <= v_n && t > 0 && t <= v_n && k != t) {
		return arr[(k-1)*v_n+t-1];
	}
	return -1;
}

void Graph::setWeight(int w, int k, int t) {
	if (this->areAdjacent(k, t) && k > 0 && k <= v_n && t > 0 && t <= v_n && k != t) {
		arr[(k-1)*v_n+t-1] = arr[(t-1)*v_n+k-1] = w;
	}
}

int Graph::num_adjacent(int k) {
	int count{}; 
	for (int i = 0; i < v_n; i++) {
		if (arr[i*v_n+k-1] != -1) {
			count++;
		}
	}
	return count;
}

bool Graph::areAdjacent(int k, int t) {
	if (arr[(k-1)*v_n+t-1] != -1) {
		return true;
	}
	return false;
}
int* Graph::adjacent(int k) {
	int n = this->num_adjacent(k);
	if (n != 0) {
		int *array = new int[n]{};
		int j = 0;
		for (int i = 0; i < v_n; i++) {
			if(arr[i*v_n+k-1] != -1) {
				array[j] = i+1;
				j++;
			}
		}
		return array;
	}
	return nullptr;
}

int Graph::all_not_last(int *array) {
	Graph g(*this);
	int *last = new int[v_n]{}, count{};
	for (int i = 1; i <= v_n; i++) {
		if (g.num_adjacent(i) == 1 && array[i-1] == 2) {
			last[count] = i;
			count++;
		}
	}
	for (int i = 0; i < count; i++) {
		g.delVertex(last[i]);
		for (int j = i+1; j < count; j++) {
			last[j]--;
		}
	}
	delete[] last;

	count = 0;
	int n = g.num_graph();
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (g.arr[i*n+j] != -1) {
				count += g.arr[i*n+j];
			}
		}
	}

	return n+count;

}
