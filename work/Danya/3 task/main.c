#include "linear_system.h"

int main(int argc, char* argv[]) {
    // Объявление всех переменных в начале
    int n, r, s, task = 16;
    char* filename = NULL;
    double** A = NULL;
    double* b = NULL;
    double* x = NULL;
    double r1 = 0.0, r2 = 0.0;
    clock_t start, end;
    double t1 = 0.0, t2 = 0.0;
    int num_threads = 4; // Количество потоков
    pthread_t* threads = NULL;
    ThreadData* thread_data = NULL;
    int i, j;
    int result;
    FILE* file;

    // Проверка аргументов командной строки
    if (argc < 4) {
        printf("Usage: %s n r s [filename]\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    r = atoi(argv[2]);
    s = atoi(argv[3]);

    if (argc > 4 && s == 0) {
        filename = argv[4];
    }

    // Выделение памяти
    A = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
    }
    b = (double*)malloc(n * sizeof(double));
    x = (double*)malloc(n * sizeof(double));

    // Инициализация матрицы
    start = clock();

    if (s == 0 && filename != NULL) {
        // Чтение из файла
        file = fopen(filename, "r");
        if (!file) {
            printf("Error opening file: %s\n", filename);
            return 1;
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (fscanf(file, "%lf", &A[i][j]) != 1) {
                    printf("Error reading matrix from file\n");
                    fclose(file);
                    return 1;
                }
            }
        }
        fclose(file);
    }
    else {
        // Инициализация по формуле с использованием потоков
        threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
        thread_data = (ThreadData*)malloc(num_threads * sizeof(ThreadData));

        for (i = 0; i < num_threads; i++) {
            thread_data[i].n = n;
            thread_data[i].A = A;
            thread_data[i].s = s;
            thread_data[i].thread_id = i;
            thread_data[i].num_threads = num_threads;
            pthread_create(&threads[i], NULL, parallel_initialize_matrix, &thread_data[i]);
        }

        for (i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        free(threads);
        free(thread_data);
    }

    // Вычисление правой части b
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    thread_data = (ThreadData*)malloc(num_threads * sizeof(ThreadData));

    for (i = 0; i < num_threads; i++) {
        thread_data[i].n = n;
        thread_data[i].A = A;
        thread_data[i].b = b;  // Потоки будут заполнять b напрямую
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        pthread_create(&threads[i], NULL, parallel_compute_b, &thread_data[i]);
    }

    // Ожидание завершения потоков
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_data);

    end = clock();
    t1 = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Вывод исходной матрицы (опционально)
    printf("Initial matrix A (first %dx%d elements):\n", (r < n) ? r : n, (r < n) ? r : n);
    print_matrix(A, n, n, r);

    // Решение системы
    start = clock();
    result = solve_jordan_row_pivot(A, b, x, n);
    end = clock();
    t1 += ((double)(end - start)) / CLOCKS_PER_SEC;

    // Вычисление невязок
    start = clock();
    if (result == 0) {
        compute_residuals(A, b, x, n, &r1, &r2);
    }
    else {
        r1 = -1.0;
        r2 = -1.0;
        t2 = 0.0;
    }
    end = clock();
    t2 = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Вывод результатов
    printf("\nSolution vector x (first %d elements):\n", (r < n) ? r : n);
    print_vector(x, n, r);

    printf("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n",
        argv[0], task, r1, r2, t1, t2, s, n);

    // Освобождение памяти
    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(x);

    return 0;
}

