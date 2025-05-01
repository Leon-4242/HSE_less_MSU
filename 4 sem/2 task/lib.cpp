// Reading and writing a matrix of dimension m*n
// m -- number of rows
// n -- number of columns
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>

// #include <sys/time.h>

const int MAX_THREADS = 16;

class ThreadArgs {
public:
    double* a;          // Pointer to matrix elements
    int rows;           // Matrix dimension: rows,
    int cols;           //                   columns 
    int resolveRow;     // Indices of resolution element: row,
    int resolveCol;     //                                column
    int row0;           // Initial row to process
    int numRows;        // Number of rows to process
    bool start;         // Request for start
    bool terminate;     // Request for suicide
    bool working;       // Thread is working

    ThreadArgs():
        a(nullptr),
        rows(0),
        cols(0),
        resolveRow(0),
        resolveCol(0),
        row0(0),
        numRows(0),
        start(false),
        terminate(false),
        working(false)
    {}
};

static std::mutex startMutex;
static std::condition_variable startCondition;

static std::mutex finishMutex;
static std::condition_variable finishCondition;

static int numThreads = 0;
static int numActiveThreads = 0;
static int numFinishedThreads = 0;
static int numWorkingThreads = 0;

static std::vector<ThreadArgs> threadArgs;
static std::vector<std::thread> threads;

void initializeThreadPool(
    double *a, int m, int n,
    int numThreads
);

void releaseThreadPool();

// Wait until all working threads finish their part of work
void waitForAllThreads();

// Thread body function
void annihilateColumn(int threadsIdx);

bool readMatrix(std::istream &f, double *&a, int &m, int &n); 
bool writeMatrix(std::ostream &f, const double *a, int m, int n);
void swapMatrixRows(
    double *a, int m, int n,
    int i, int k
);
void addMatrixRows(         // row[k] += row[i]*coeff 
    double *a, int m, int n,
    int k, int i, double coeff
);
void multiplyMatrixRow(
    double *a, int m, int n,
    int i, double coeff
);

double* product(const double *a, int n, const double*b);

int parallelGaussUpMethod(    // Return a matrix rank
    double *a, int m, int n,
    int numThreads = 4
);

double det(double *a, int m, int n);

bool solveLinearSystem(
    const double *a, int n, // matrix n*n
    const double* b,        // column of the system right side
    double* &x,              // Solution of the system a*x = b
	int numThreads
);

bool inverseMatrix(
    const double *a, int n, // matrix n*n
    double* &inverse,       // out: inverse matrix
	int numThreads
);

const double EPS = 1e-8;

int main() {
    int m, n, switcher;
    double *a = NULL;

	std::cout << "Enter number of task:\n1)Inverse matrix\n2)Linear system" << std::endl;
	std::cin >> switcher;

	if (switcher != 1 && switcher != 2) return -1;

	std::cout << "Enter number of threads: " << std::endl;
    if (!(std::cin >> numThreads)) return -1;

	std::ifstream f("input.txt", std::ios::in);
    if (!f.is_open()) {
		std::cerr << "Cannot open an input file.";
        return (-1);
    }
	
	if (switcher == 1) {
		if (!readMatrix(f, a, m, n)) {
			std::cout << "Incorrect format of matrix.\n" << std::endl;
		    delete[] a;
		    f.close();
		    return -1;
		}
		f.close();
		if (n != m) {
			std::cout << "Matrix is not square." << std::endl;
			delete[] a;
			return -1;
		}
		auto begin = std::chrono::high_resolution_clock::now();

		double* inverse = NULL;
		if(!inverseMatrix(a, n, inverse, numThreads)) {
			std::cout << "Matrix is degenerate" << std::endl;
			delete[] a;
			return -1;
		}

		auto end = std::chrono::high_resolution_clock::now();

		auto elapsed = std::chrono::duration<double, std::milli>(end - begin);
		
		printf("Row echelon form of matrix:\n");
		writeMatrix(std::cout, inverse, n, n);
		std::cout << "Computation time: " << elapsed.count() << "ms." << std::endl;

		std::ofstream g("output.txt", std::ios::out);
		writeMatrix(g, inverse, n, n);
		delete[] inverse;

	} else {

		if (!readMatrix(f, a, m, n)) {
			std::cout << "Incorrect format of matrix.\n" << std::endl;
		    delete[] a;
		    f.close();
		    return -1;
		}
		if (m != n) {
			std::cout << "Matrix is not square." << std::endl;
			delete[] a;
			f.close();
			return -1;
		}
		double *b = NULL; int k, t;
		if (!readMatrix(f, b, k, t)) {
			std::cout << "Incorrect format of b." << std::endl;
			delete[] a;
			f.close();
			return -1;
		}
		std::cout << "\n" << k << " " << t << std::endl;
		if (k != n || t != 1) {
			std::cout << "b is incorrect colomn." << std::endl;
			delete[] a;
			delete[] b;
			f.close();
			return -1;
		}

		f.close();

		auto begin = std::chrono::high_resolution_clock::now();

		double *x = NULL;
		if (!solveLinearSystem(a, n, b, x, numThreads)) {
			std::cout << "Matrix is not square or matrix is degenerate." << std::endl;
			delete[] a;
			delete[] b;
			return -1;
		}

		auto end = std::chrono::high_resolution_clock::now();

		auto elapsed = std::chrono::duration<double, std::milli>(end - begin);
		
		printf("Row echelon form of matrix:\n");
		writeMatrix(std::cout, x, n, 1);
		std::cout << "Computation time: " << elapsed.count() << "ms." << std::endl;

		std::ofstream g("output.txt", std::ios::out);
		writeMatrix(g, x, n, 1);
		
		delete[] x;
	}
    
    return 0;
}

bool readMatrix(std::istream &f, double *&a, int &m, int &n) {
    // 2 3     
    // 11 12 13
    // 21 22 23
    int rows, cols;
    if (!(f >> rows >> cols)) {
        return false;
    }
    m = rows; n = cols;
    double *matrix = new double[rows*cols];
    a = matrix;
    for (int i = 0; i < rows; ++i) {
        // Read i-th row of matrix
        for (int j = 0; j < cols; ++j) {
            if (!(f >> matrix[i*cols+j])) {
                // Read error
                return false;
            }
        }
    }
    return true;
}
    
bool writeMatrix(std::ostream &f, const double *a, int m, int n) {
    if (!(f << m << " "<< n << "\n")) {
        return false;
    } 
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
			if (!(f << std::setprecision(3) << std::setw(10) << a[i*n+j])) return false;
        }
        f << "\n";
    }    
    return true;
}

int parallelGaussUpMethod(    // Return a matrix rank
    double *a, int m, int n,
    int numThreads
) {  
    assert(numThreads > 0);
    if (numThreads > MAX_THREADS)
        numThreads = MAX_THREADS;
    
    threadArgs.resize(numThreads);

    initializeThreadPool(
        a, m, n,
        numThreads
    ); 

    int i = 0;
    int j = 0;
    while (i < m && j < n) {
        // 1. Find the maximal element in j-th column
        int k = i;
        double maxElem = fabs(a[k*n + j]);
        for (int l = i+1; l < m; ++l) {
            if (fabs(a[l*n + j]) > maxElem) {
                maxElem = fabs(a[l*n + j]);
                k = l;
            }
        }
        
        if (maxElem <= EPS) {
            // Zero colunm
            for (int l = i; l < m; ++l) {
                a[l*n + j] = 0.;
            }
            ++j;
            continue;
        }
        
        if (k != i) {
            swapMatrixRows(a, m, n, i, k);
        }            
	
		multiplyMatrixRow(
			a, m, n,
			i, 1/a[i*n+j]
		);

        // Distribute tasks among threads
        int rowsToProcess = m - (i + 1);
        if (rowsToProcess > 0) {
            {
                std::lock_guard<std::mutex> lock(finishMutex);
                numFinishedThreads = 0;
            }

            {
                std::lock_guard<std::mutex> lock(startMutex);

                numActiveThreads = numThreads;
                if (numActiveThreads > rowsToProcess)
                    numActiveThreads = rowsToProcess;
                int rowsPerThread = rowsToProcess/numActiveThreads;
                if (rowsToProcess%numActiveThreads != 0) {
                    ++rowsPerThread;
                    numActiveThreads = rowsToProcess/rowsPerThread;
                    if (rowsToProcess%rowsPerThread > 0)
                        ++numActiveThreads;
                }

                /*
                printf(
                    "Number of active threads=%d, rowsPerThread=%d\n",
                    numActiveThreads, rowsPerThread
                );
                */

                int row = i+1;
                for (int t = 0; t < numActiveThreads; ++t) {
                    assert(row < m);
                    ThreadArgs* args = &(threadArgs[t]);
                    args->resolveRow = i;
                    args->resolveCol = j;
                    args->row0 = row;
                    int nRows = rowsPerThread;
                    if (row + nRows > m) {
                        nRows = m - row;
                    }
                    args->numRows = nRows;
                    args->start = true;
                    row += nRows;
                }
                assert(row == m);
            }

            startCondition.notify_all();

            waitForAllThreads();

		}
		
		rowsToProcess = i;
        if (rowsToProcess > 0) {
            {
                std::lock_guard<std::mutex> lock(finishMutex);
                numFinishedThreads = 0;
            }

            {
                std::lock_guard<std::mutex> lock(startMutex);

                numActiveThreads = numThreads;
                if (numActiveThreads > rowsToProcess)
                    numActiveThreads = rowsToProcess;
                int rowsPerThread = rowsToProcess/numActiveThreads;
                if (rowsToProcess%numActiveThreads != 0) {
                    ++rowsPerThread;
                    numActiveThreads = rowsToProcess/rowsPerThread;
                    if (rowsToProcess%rowsPerThread > 0)
                        ++numActiveThreads;
                }

                /*
                printf(
                    "Number of active threads=%d, rowsPerThread=%d\n",
                    numActiveThreads, rowsPerThread
                );
                */

                int row = 0;
                for (int t = 0; t < numActiveThreads; ++t) {
                    assert(row < i);
                    ThreadArgs* args = &(threadArgs[t]);
                    args->resolveRow = i;
                    args->resolveCol = j;
                    args->row0 = row;
                    int nRows = rowsPerThread;
                    if (row + nRows > i) {
                        nRows = i - row;
                    }
                    args->numRows = nRows;
                    args->start = true;
                    row += nRows;
                }
                assert(row == i);
            }

            // printf("notify_all to start\n");

            startCondition.notify_all();

            waitForAllThreads();

        } // end if (rowsToProcess > 0)

        ++i; ++j;
    } // end while
    
    releaseThreadPool();
    
    return i;
}
            
double det(double *a, int m, int n) {
    double d = 1.;
    int k = n;
    if (m < n)
        k = m;
    for (int i = 0; i < k; ++i) {
        d *= a[i*n + i];
    }
    return d;
}

void initializeThreadPool(
    double *a, int m, int n,
    int numThreads
) {
    {
        std::lock_guard<std::mutex> lock(finishMutex);
        numWorkingThreads = 0;
    }

    for (int threadIdx = 0; threadIdx < numThreads; ++threadIdx) {
        ThreadArgs* arg = &(threadArgs[threadIdx]);
        arg->a = a;
        arg->rows = m;
        arg->cols = n;
        arg->resolveRow = 0;
        arg->resolveCol = 0;
        arg->row0 = 0;
        arg->numRows = 0;
        arg->start = false;
        arg->terminate = false;
        arg->working = false;
        threads.push_back(
            std::thread(annihilateColumn, threadIdx)
        );
    }

    {
        std::unique_lock<std::mutex> lock(finishMutex);
        while (numWorkingThreads < numThreads) {
            // printf("numWorkingThreads=%d, waiting...\n", numWorkingThreads);
            finishCondition.wait(lock);
        }
        /*
        printf(
            "numWorkingThreads=%d, all threads started\n", 
            numWorkingThreads
        );
        */
    }
}

void releaseThreadPool() {
    {
        std::lock_guard<std::mutex> lock(startMutex);
        for (int i = 0; i < numThreads; ++i) {
            threadArgs[i].terminate = true;
        }
    }

    // printf("notify_all to terminate.\n");

    startCondition.notify_all();

    for (int i = 0; i < numThreads; ++i) {
        // printf("joining thread %d.\n", i);
        threads[i].join();
    }
}

void annihilateColumn(int threadIdx) {
    ThreadArgs& threadArg = threadArgs[threadIdx];
    double* a = threadArg.a;
    int m = threadArg.rows;
    int n = threadArg.cols;

    // printf("Thread is created, idx=%d\n", threadIdx);
    {
        std::lock_guard<std::mutex> lock(finishMutex);
        ++numWorkingThreads;
    }
    finishCondition.notify_one();

    while (true)
    {
        bool endWork = false;
        {
            std::unique_lock<std::mutex> lock(startMutex);
            while (true) {
                if (threadArg.terminate) {
                    endWork = true;
                    break;
                } else if (threadIdx < numActiveThreads && threadArg.start) {
                    break;
                }

                // printf("Thread %d, waiting for start...\n", threadIdx);
                startCondition.wait(lock);
            }
        }

        if (endWork) {
            // printf("Thread %d, terminating\n", threadIdx);
            break;
        }
        
        // printf("Thread is started, idx=%d\n", threadIdx);

        assert(threadArg.start);
        threadArg.start = false;
        assert(threadArg.numRows > 0);
        
		int i = threadArg.resolveRow;
		int j = threadArg.resolveCol;
		double r = a[i*n + j];
		int row0 = threadArg.row0;
		int row1 = row0 + threadArg.numRows;
        
		/*
		printf(
		    "ThreadIdx=%d: process row0=%d, numRows=%d, i=%d, j=%d\n",
		    threadIdx, row0, threadArg.numRows, i, j
		);
		*/
        
		for (int k = row0; k < row1; ++k) {
		    // Subtract i-th row multiplied by coeff from k-th row
		    //     so that the element a_kj becomes zero
		    double coeff = (-a[k*n + j]/r);
		    addMatrixRows(
		        a, m, n,
		        k, i, coeff
		    );
		    a[k*n + j] = 0.;
		}
				
		{
		    std::lock_guard<std::mutex> lock(finishMutex);
		    ++numFinishedThreads;
		}

        finishCondition.notify_one(); // Signal the end of work

    } // end while

    // printf("Thread terminated, ID=%d\n", threadArg.threadIdx);
}

void waitForAllThreads() {
    std::unique_lock<std::mutex> lock(finishMutex);
    while (numFinishedThreads < numActiveThreads) {
        /*
        printf(
            "numFinishedThreads = %d, waiting for %d\n",
            numFinishedThreads, numActiveThreads
        );
        */
        finishCondition.wait(lock);
    }
    // printf("waitForAllThreads finished.\n");
}

void swapMatrixRows(
    double *a, int m, int n,
    int i, int k
) {
    assert(0 <= i && i < m);
    assert(0 <= k && k < m);
    for (int j = 0; j < n; ++j) {
        double tmp = a[i*n + j];
        a[i*n + j] = a[k*n + j];
        a[k*n + j] = (-tmp);
    }
}

void addMatrixRows(         // row[k] += row[i]*coeff 
    double *a, int m, int n,
    int k, int i, double coeff
) {
    assert(0 <= i && i < m);
    assert(0 <= k && k < m);
    for (int j = 0; j < n; ++j) {
        a[k*n + j] += a[i*n + j]*coeff;
    }
}

void multiplyMatrixRow(
    double *a, int m, int n,
    int i, double coeff
) {
    assert(0 <= i && i < m);
    for (int j = 0; j < n; ++j) {
        a[i*n + j] *= coeff;
    }
}

bool inverseMatrix(
    const double *a, int n, // matrix n*n
    double* &inverse,		// out: inverse matrix
	int numThreads
) {
	double* tmp = new double[n*2*n];

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < 2*n; ++j) {
			if (j < n) tmp[i*2*n+j] = a[i*n+j];
			else tmp[i*2*n+j] = ((i+n == j) ? 1 : 0);
		}
	}

	int rank = parallelGaussUpMethod(tmp, n, 2*n, numThreads);

	if (rank != n) {
		delete[] tmp;
		return false;
	}

	inverse = new double[n*n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			inverse[i*n+j] = tmp[i*2*n+j+n];
		}
	}

	delete[] tmp;
	return true;
}

bool solveLinearSystem(
    const double *a, int n, // matrix n*n
    const double* b,        // column of the system right side
    double* &x,             // Solution of the system a*x = b
	int numThreads
) {
	double* tmp;
	if (!inverseMatrix(a, n, tmp, numThreads)) return false;
	x = product(tmp, n, b);
	delete[] tmp;
	return true;
}

double* product(const double *a, int n, const double*b) {
	double *res = new double[n];
	double sum = 0;
	for (int i = 0; i < n; ++i) {
		sum = 0;
		for (int j = 0; j < n; ++j) sum += a[i*n+j]*b[j];
		res[i] = sum;
	}
	return res;
}
