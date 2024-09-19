#include <iostream>
using namespace std;

class Graph {
	int v_n;
	int *arr;
	
	public:
	
	Graph() {
		v_n = 0;
		arr = nullptr;
	}

	Graph(int k) {
		v_n = k;
		arr = new int[k*k]{};
		for (int i = 0; i < v_n*v_n; i++) {
			arr[i] = -1;
		}
	}
	
	Graph(const Graph &g) {
		v_n = g.v_n;
		arr = new int[v_n*v_n]{};
		for (int i = 0; i < v_n*v_n; i++) {
			arr[i] = g.arr[i];
		}
	}
	int num_graph(void);
	void addVertex(void);
	void addEdge(int, int);

	void delVertex(int);
	void delEdge(int, int);

	void print(void);
	int Weight(int, int);
	void setWeight(int, int, int);

	bool areAdjacent(int, int);
	int num_adjacent(int);
	int* adjacent(int);
	int all_not_last(int*);
	~Graph() {
		delete[] arr;
	}

};

void compact(Graph*, int*, int);
bool arecompact(int*, int, Graph, int*);
int Weight_of_last(Graph, int*);

int main(void) {
	int n = 0, n_1 = 0, *arr = nullptr;
	cin >> n >> n_1;
	Graph g(n);

	arr = new int[n];
	for (int i = 0; i < n; i++) cin >> arr[i];

	for (int i = 0, k = 0, t = 0; i < n_1; i++) {
		cin >> k >> t;
		g.addEdge(k, t);
	}
	
	compact(&g, arr, n);
	int sum = g.all_not_last(arr);
	sum += Weight_of_last(g, arr);
	cout << sum << endl;
	
	delete[] arr;
	return 0;
}

int Weight_of_last(Graph g, int *array) {
	int n = g.num_graph(), k = 0;
	for (int i = 1; i <= n; i++) {
		if (g.num_adjacent(i) == 1 && array[i-1] == 2) {
			k++;
		}
	}

	if (k == 0) {
		return 0;
	} 
	int *arr = new int[k];
	int j = 0;
	for (int i = 1; i <= n; i++) {
		if (g.num_adjacent(i) == 1 && array[i-1] == 2) {
			arr[j] = i;
			j++;
		}
	}
	int *temp = g.adjacent(arr[0]);
	int max = g.Weight(temp[0], arr[0]);
	delete[] temp;

	if (k == 1) {
		delete[] arr;
		return max+1;
	} 
	
	temp = g.adjacent(arr[1]);
	int max_1 = g.Weight(temp[0], arr[1]);
	delete[] temp;
	int curr = 0;
	if (max < max_1) {
		int temp = max;
		max = max_1;
		max_1 = temp;
	}
	for (int i = 2; i < k; i++) {
		temp = g.adjacent(arr[i]);
		curr = g.Weight(temp[0], arr[i]);
		delete[] temp;
		if (curr > max) {
			max_1 = max;
			max= curr;
		} else if (curr > max_1) {
			max_1 = curr;
		}
	}
	delete[] arr;
	return max+max_1+2;
}

bool arecompact(int *arr, int n, Graph g, int * ver) {
	for (int i = 1; i <= n;  i++) {
		if (g.num_adjacent(i) == 1 && arr[i-1] == 2) {			
			int *array = g.adjacent(i);
			if (g.num_adjacent(array[0]) == 2) {
				*ver = i;
				delete[] array;
				return true;
			}
			delete[] array;
		}
	}
	return false;
}

void compact(Graph *g, int *arr, int n) {
	int curr = 0, *array = nullptr;
	while (arecompact(arr, n, *g, &curr)) {
		array = g->adjacent(curr);
		int *temp = g->adjacent(array[0]);
		int k = temp[0]; if (k == curr) {k = temp[1];}

		delete[] temp;
		g->setWeight(1+g->Weight(array[0], curr), array[0], k);
		g->delVertex(curr);
		delete[] array;
	}
}

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
