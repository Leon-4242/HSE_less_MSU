#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 100

// Структура для передачи аргументов в поток
typedef struct {
    sem_t* fork_l;
	sem_t* fork_r;
	int n;
} ThreadArgs;


// Функция, которую выполняет каждый пекарь
void* eat(void* arg) {
    ThreadArgs* args;  int time = 0;

   	args = (ThreadArgs*)arg;
	time = 3;

	while(1) {
		sleep(time);

		sem_wait(args->fork_l);
		printf("\ntaked left (%d)", args->n);
		sem_wait(args->fork_r);
		printf("\ntaked right (%d)", args->n);
		
		sleep(time);
		
		sem_post(args->fork_l);
		printf("\nput left");
		sem_post(args->fork_r);
		printf("\nput right");

		printf("\nI ate? Yes, I ate.");
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	int NumberPhilosophers = 0, i = 0;
	pthread_t* threads;
	ThreadArgs* forks;

    if (argc != 2) {
        fprintf(stderr, "Wrong data.\nNeed: NumberPhilosophers");
        return 1;
    }
	srand(time(NULL));

    NumberPhilosophers = atoi(argv[1]);

	if (NumberPhilosophers <= 1 || NumberPhilosophers > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads. Must be between 2 and %d.\n", MAX_THREADS);
        return 1;
    }

	threads = (pthread_t*)malloc(NumberPhilosophers*sizeof(pthread_t));
	forks = (ThreadArgs*)malloc(NumberPhilosophers*sizeof(ThreadArgs));
	for (i = 0; i < NumberPhilosophers; ++i){
		(forks+i)->fork_l = (sem_t*)malloc(sizeof(sem_t));
		(forks+i)->n = i;
		if (i > 0) {
			(forks+i-1)->fork_r = (forks+i)->fork_l;
		}
	}
	(forks+NumberPhilosophers-1)->fork_r = forks->fork_l;

    // Инициализация семафора
	for (i = 0; i < NumberPhilosophers; ++i) {
		if(sem_init((forks+i)->fork_l, 0, 1) != 0) {
		    fprintf(stderr, "Failed to initialize semaphore");
			return 1;
		}
	}

	for (i = 0; i < NumberPhilosophers; ++i) {
		if ((forks+i)->fork_r != (forks+(i+1)%NumberPhilosophers)->fork_l) printf("\nerror in i = %d", i);
	}

    // Создание потоков
	for (i = 0; i < NumberPhilosophers; i++) {
        if (pthread_create(threads+i, NULL, eat, forks+i) != 0) {
            fprintf(stderr, "Failed to create \"Philosopher\".\n");
            return 1;
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < NumberPhilosophers; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    // Уничтожение семафора
	for (i = 0; i < NumberPhilosophers; ++i) {
		sem_destroy((forks+i)->fork_l);
		free((forks+i)->fork_l);
	}	

	free(forks);
	free(threads);
    return 0;
}

