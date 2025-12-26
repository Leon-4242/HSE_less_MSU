#include <chrono>
#include "input.h"
#include "output.h"
#include "lu_linear_parallel.h"

int main(int argc, char* argv[]) {
	int l = 0;
	std::string filename; 
	double r1 = 0, r2 = 0, t2 = 0, sum = 0, tmp = 0;
	
	ThreadArgs* args; 
	pthread_t* threads; 
	pthread_barrier_t barrier;
	pthread_mutex_t mutex;

	int task = 4;

	//Initialization
	if (argc < 5 || argc > 6) {
		std::cout << "Invalid number of arguments.\n" << std::endl;
		return -1;
	}

	int n = std::stoi(argv[1]);
	int p = std::stoi(argv[2]);
	int r = std::stoi(argv[3]);
	int s = std::stoi(argv[4]);

	if (s == 0 && argc == 5) {
		std::cout << "Invalid number of arguments.\n" << std::endl;
		return -1;
	}

	if (s == 0) 
		filename = std::string(argv[5]);
	
	double* array = new double[n*n];

	int res = input(s, filename, n, array);
	switch (res) {
		case -1:
			std::cout << "Error during opening file " << filename << std::endl;
			delete[] array;
			return -1;
		case -2:
			std::cout << "Wrong data in file " << filename << std::endl;
			delete[] array;
			return -1;
		case -3:
			std::cout << "Wrong type of formule: " << s << std::endl;
			delete[] array;
			return -1;
	}

	double* b = new double[n];

	for (int i = 0; i < n; ++i) {
		b[i] = 0;
		for (int k = 0; k < n; k += 2) {
			b[i] += array[i*n+k];
		}
	}

	int* indi = new int[n];
	for (int i = 0; i < n; ++i) 
		indi[i] = i;

	output(r, n, n, array);
	std::cout << "\n" << std::endl;

	double* result = new double[n];
    
	// REPAIR

	args = new ThreadArgs[p];
    threads = new pthread_t[p];
	
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, p);

	auto start_time = std::chrono::steady_clock::now();
	
	for (int i = 0; i < p; ++i) {
		(args+i)->a = array;
		(args+i)->b = b; 
		(args+i)->res = result;
		(args+i)->n = n;
		(args+i)->ind = indi;

		(args+i)->numThreads = p;
		(args+i)->thread_id = i;
		(args+i)->barrier = &barrier;
		(args+i)->s = &tmp;
		(args+i)->k = &l;
		(args+i)->mutex = &mutex;
		if (pthread_create(threads + i, NULL, lu_linear_parallel, args+i) != 0)
			return 1;
	}
	
	double* buff = NULL;
	bool flag = false;

    for (int i = 0; i < p; ++i) {
        if (pthread_join(threads[i], (void**)&buff) != 0) {
			std::cout << "Failed to join thread" << std::endl;
            return 1;
        }

		if (buff != NULL) 
			flag = true;
    }

	// REPAIR

	auto end_time = std::chrono::steady_clock::now();
    double t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	res = input(s, filename, n, array);	
	
	for (int i = 0; i < n; ++i) {
		b[i] = 0;
		for (int k = 0; k < n; k += 2) {
			b[i] += array[i*n+k];
		}
	}
	
	if (!flag) {
		output(r, 1, n, b);
		std::cout << "\n" << std::endl;
		output(r, 1, n, result);

		auto start_time = std::chrono::steady_clock::now();
		
		r1_r2(n, array, result, b, r1, r2);
		
		auto end_time = std::chrono::steady_clock::now();
		t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	} else {
		r1 = -1; r2 = -1; t2 = 0;
	}

	std::cout << 
		argv[0] << " : Task = " << task << 
		std::scientific << " Res1 = " << r1 << " Res2 = " << r2 << 
		std::defaultfloat << std::fixed << std::setprecision(2) << " T1 = " << t1 << " T2 = " << t2 << 
		std::defaultfloat << " S = " << s << " N = "<< n << 
	std::endl;
	
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);
	
	delete[] array;
	delete[] result;
	delete[] b;
	delete[] indi;

	delete[] args;
	delete[] threads;

	return 0;
}
