// ==== linear_system.c ====
#include "linear_system.h"
#include <string.h>

// Формула для элемента матрицы
double formula_element(int s, int n, int i, int j)
{
    double result;
    
    switch(s) {
        case 1: result = n - fmax(i, j) + 1.0; break;
        case 2: result = fmax(i, j); break;
        case 3: result = fabs((double)(i - j)); break;
        case 4: result = 1.0 / (i + j - 1); break;
        default: result = 0.0; break;
    }
    
    return result;
}

// Параллельная инициализация матрицы
void* parallel_initialize_matrix(void* arg)
{
    ThreadData* data;
    int start;
    int end;
    int i;
    int j;
    
    data = (ThreadData*)arg;
    start = data->thread_id * data->n / data->num_threads;
    end = (data->thread_id + 1) * data->n / data->num_threads;
    
    for(i = start; i < end; i++) {
        for(j = 0; j < data->n; j++) {
            if(data->s == 0) {
                data->A[i][j] = 0.0;
            } else {
                data->A[i][j] = formula_element(data->s, data->n, i+1, j+1);
            }
        }
    }
    
    return NULL;
}

// Параллельное вычисление правой части
void* parallel_compute_b(void* arg)
{
    ThreadData* data;
    int start;
    int end;
    int i;
    int k;
    int col;
    
    data = (ThreadData*)arg;
    start = data->thread_id * data->n / data->num_threads;
    end = (data->thread_id + 1) * data->n / data->num_threads;
    
    for(i = start; i < end; i++) {
        data->b[i] = 0.0;
        for(k = 0; k <= (data->n + 1)/2 - 1; k++) {
            col = 2*k;
            if(col < data->n) {
                data->b[i] += data->A[i][col];
            }
        }
    }
    
    return NULL;
}

// Метод Жордана с выбором главного элемента по строке (задача 16)
int solve_jordan_row_pivot(double** A, double* b, double* x, int n)
{
    int i;
    int j;
    int k;
    int pivot_row;
    double max_val;
    double factor;
    double pivot;
    double** A_copy;
    double* b_copy;
    int* row_perm;
    
    // Создаем копии матрицы и вектора
    A_copy = (double**)malloc(n * sizeof(double*));
    b_copy = (double*)malloc(n * sizeof(double));
    for(i = 0; i < n; i++) {
        A_copy[i] = (double*)malloc(n * sizeof(double));
        b_copy[i] = b[i];
        for(j = 0; j < n; j++) {
            A_copy[i][j] = A[i][j];
        }
    }
    
    row_perm = (int*)malloc(n * sizeof(int));
    for(i = 0; i < n; i++) {
        row_perm[i] = i;
    }
    
    for(k = 0; k < n; k++) {
        // Выбор главного элемента в строке (по строке)
        pivot_row = k;
        max_val = fabs(A_copy[row_perm[k]][k]);
        
        for(i = k + 1; i < n; i++) {
            if(fabs(A_copy[row_perm[i]][k]) > max_val) {
                max_val = fabs(A_copy[row_perm[i]][k]);
                pivot_row = i;
            }
        }
        
        if(max_val < 1e-12) {
            // Освобождаем память
            for(i = 0; i < n; i++) {
                free(A_copy[i]);
            }
            free(A_copy);
            free(b_copy);
            free(row_perm);
            return 1; // Матрица вырожденная
        }
        
        // Обмен строк
        if(pivot_row != k) {
            int temp = row_perm[k];
            row_perm[k] = row_perm[pivot_row];
            row_perm[pivot_row] = temp;
        }
        
        // Нормализация текущей строки
        pivot = A_copy[row_perm[k]][k];
        for(j = k; j < n; j++) {
            A_copy[row_perm[k]][j] /= pivot;
        }
        b_copy[row_perm[k]] /= pivot;
        
        // Исключение для всех других строк
        for(i = 0; i < n; i++) {
            if(i != k) {
                factor = A_copy[row_perm[i]][k];
                for(j = k; j < n; j++) {
                    A_copy[row_perm[i]][j] -= factor * A_copy[row_perm[k]][j];
                }
                b_copy[row_perm[i]] -= factor * b_copy[row_perm[k]];
            }
        }
    }
    
    // Восстановление решения
    for(i = 0; i < n; i++) {
        x[i] = b_copy[row_perm[i]];
    }
    
    // Освобождаем память
    for(i = 0; i < n; i++) {
        free(A_copy[i]);
    }
    free(A_copy);
    free(b_copy);
    free(row_perm);
    
    return 0;
}

// Вычисление невязок
void compute_residuals(double** A, double* b, double* x, int n, double* r1, double* r2)
{
    int i;
    int j;
    double norm_b;
    double norm_x;
    double norm_Ax_b;
    double norm_x_xhat;
    double sum;
    double xhat_i;
    
    norm_b = 0.0;
    norm_x = 0.0;
    norm_Ax_b = 0.0;
    norm_x_xhat = 0.0;
    
    // Вычисление нормы b
    for(i = 0; i < n; i++) {
        norm_b += fabs(b[i]);
    }
    
    if(norm_b < 1e-12) norm_b = 1e-12;
    
    // Вычисление нормы x и x_hat
    for(i = 0; i < n; i++) {
        norm_x += fabs(x[i]);
        xhat_i = (i % 2 == 0) ? 1.0 : 0.0;
        norm_x_xhat += fabs(x[i] - xhat_i);
    }
    
    if(norm_x < 1e-12) norm_x = 1e-12;
    
    // Вычисление Ax - b
    for(i = 0; i < n; i++) {
        sum = 0.0;
        for(j = 0; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        norm_Ax_b += fabs(sum - b[i]);
    }
    
    *r1 = norm_Ax_b / norm_b;
    *r2 = norm_x_xhat / norm_x;
}

// Печать матрицы
void print_matrix(double** matrix, int rows, int cols, int r)
{
    int print_rows;
    int print_cols;
    int i;
    int j;
    
    print_rows = (rows < r) ? rows : r;
    print_cols = (cols < r) ? cols : r;
    
    for(i = 0; i < print_rows; i++) {
        for(j = 0; j < print_cols; j++) {
            printf(" %10.3e", matrix[i][j]);
        }
        printf("\n");
    }
}

// Печать вектора
void print_vector(double* vector, int n, int r)
{
    int print_len;
    int i;
    
    print_len = (n < r) ? n : r;
    
    for(i = 0; i < print_len; i++) {
        printf(" %10.3e", vector[i]);
    }
    printf("\n");
}