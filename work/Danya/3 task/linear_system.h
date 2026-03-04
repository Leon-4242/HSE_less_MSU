#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int n;
    double** A;
    double* b;
    double* x;
    int s;
    int task;
    int r;
    int thread_id;
    int num_threads;
    double* partial_sum;
    int* pivot;
} ThreadData;

// Инициализация матрицы
void initialize_matrix(double** A, int n, int s, const char* filename);

// Формулы для инициализации элементов матрицы
double formula_element(int s, int n, int i, int j);

// Вычисление правой части b
void compute_b(double** A, double* b, int n);

// Решение системы методом Жордана с выбором главного элемента по всей матрице
int solve_jordan_row_pivot(double** A, double* b, double* x, int n);

// Вычисление невязок для линейной системы
void compute_residuals(double** A, double* b, double* x, int n, double* r1, double* r2);

// Печать матрицы/вектора
void print_matrix(double** matrix, int rows, int cols, int r);
void print_vector(double* vector, int n, int r);

// Параллельные функции
void* parallel_initialize_matrix(void* arg);
void* parallel_compute_b(void* arg);
void* parallel_jordan_elimination(void* arg);

#endif