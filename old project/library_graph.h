#ifndef LIB
#define LIB

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

#endif
