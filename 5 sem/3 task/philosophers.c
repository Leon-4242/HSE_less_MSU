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

void* eat(void*);
void* chosen_eat(void*);

// Функция, которую выполняет каждый философ
void* eat(void* arg) {
    ThreadArgs* args;

   	args = (ThreadArgs*)arg;
//	time = (rand()%5)+1;

	while(1) {
		sleep(rand()%2);
		sem_wait(args->fork_l);
		printf("\n Philosopher number %d took left fork\n", args->n);
		sleep(rand()%2);
		sem_wait(args->fork_r);
		printf("\n Philosopher number %d took right fork\n", args->n);
	
		sleep(rand()%2);
		printf("\nI ate? Yes, I ate. (%d)", args->n);

		sem_post(args->fork_r);
		sem_post(args->fork_l);
	}
	return NULL;
}

void* chosen_eat(void* arg) {
    ThreadArgs* args;

   	args = (ThreadArgs*)arg;
//	time = (rand()%5)+1;

	while(1) {
		sleep(rand()%2);
		sem_wait(args->fork_r);
		printf("\n Chosen philosopher (number %d) took right fork\n", args->n);
		sleep(rand()%2);
		sem_wait(args->fork_l);
		printf("\n Chosen philosopher (number %d) took left fork\n", args->n);

		sleep(rand()%2);
		printf("\nI ate? Yes, I ate. (%d)", args->n);


		sem_post(args->fork_l);
		sem_post(args->fork_r);
	}
	return NULL;

}

int main(int argc, char* argv[]) {
	int NumberPhilosophers = 0, i = 0, chosen_number = 0;
	pthread_t* threads; sem_t* forks;
	ThreadArgs* args;

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
	forks = (sem_t*)malloc(NumberPhilosophers*sizeof(sem_t));
	args = (ThreadArgs*)malloc(NumberPhilosophers*sizeof(ThreadArgs));

    // Инициализация семафора
	for (i = 0; i < NumberPhilosophers; ++i) {
		if(sem_init(forks+i, 0, 1) != 0) {
		    fprintf(stderr, "Failed to initialize semaphore");
			return 1;
		}
	}

	for (i = 0; i < NumberPhilosophers; ++i) {
		(args+i)->n = i;
		(args+i)->fork_l = forks+i;
		(args+i)->fork_r = forks+(i+1)%NumberPhilosophers;
	}

	chosen_number = rand()%NumberPhilosophers;
    // Создание потоков
	for (i = 0; i < NumberPhilosophers; i++) {
        if (pthread_create(threads+i, NULL, (i == chosen_number) ? chosen_eat : eat, args+i) != 0) {
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
		sem_destroy((forks+i));
	}	

	free(forks);
	free(args);
	free(threads);
    return 0;
}

