#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 100

// Структура для передачи аргументов в поток
typedef struct {
    pthread_mutex_t* fork_l;
	pthread_mutex_t* fork_r;
	sem_t* glutton_sem;
	int n;
	int* hunger;
} ThreadArgs;


void* eat(void*);
void* glutton_eat(void*);

// Функция, которую выполняет каждый философ
void* eat(void* arg) {
    ThreadArgs* args;

   	args = (ThreadArgs*)arg;
//	time = (rand()%5)+1;

	while(1) {
//		sleep(rand()%2);
//		if (pthread_mutex_trylock(args->fork_l)) {
//			++(*args->hunger);
//			sleep(rand()%2);
//			continue;
//		}	
		pthread_mutex_lock(args->fork_l);
//		printf("\nPhilosopher number %d took left fork\n", args->n);
//		sleep(rand()%2);
		if (pthread_mutex_trylock(args->fork_r)) {
			pthread_mutex_unlock(args->fork_l);
			++(*(args->hunger));
			sleep(rand()%2);
		} else {
//			printf("\nPhilosopher number %d took right fork\n", args->n);
			*(args->hunger) = 0;
			sleep(rand()%2);

			pthread_mutex_unlock(args->fork_l);
			pthread_mutex_unlock(args->fork_r);
		}
	}
	return NULL;
}

void* glutton_eat(void* arg) {
	ThreadArgs* args;
	args = (ThreadArgs*)arg;

	while(1) {
		//sleep(rand()%2);
		
		pthread_mutex_lock(args->fork_l);
//		printf("\nGlutton number %d took left fork\n", args->n);
		//sleep(rand()%2);
		pthread_mutex_lock(args->fork_r);
//		printf("\nGlutton number %d took right fork\n", args->n);
		sem_post(args->glutton_sem);

		*(args->hunger) = 0;
		sleep(rand()%2);

		sem_wait(args->glutton_sem);

		pthread_mutex_unlock(args->fork_l);
//		printf("\nGlutton number %d put left fork\n", args->n);
		pthread_mutex_unlock(args->fork_r);
//		printf("\nGlutton number %d put right fork\n", args->n);
	}
}

int main(int argc, char* argv[]) {
	int NumberPhilosophers = 0, NumberGluttons = 0, i = 0;
	pthread_t* threads; pthread_mutex_t* forks;  sem_t sem;
	ThreadArgs* args; int* hunger;

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
	forks = (pthread_mutex_t*)malloc(NumberPhilosophers*sizeof(pthread_mutex_t));
	args = (ThreadArgs*)malloc(NumberPhilosophers*sizeof(ThreadArgs));
	hunger = (int*)malloc(NumberPhilosophers*sizeof(int));

    // Инициализация семафора
	for (i = 0; i < NumberPhilosophers; ++i) {
		if(pthread_mutex_init(forks+i, NULL) != 0) {
		    fprintf(stderr, "Failed to initialize mutex");
			return 1;
		}
		hunger[i] = 0;
	}

	if (sem_init(&sem, 0, 0) != 0) {
		fprintf(stderr, "Failed to initialize sem");
		return 1;
	}

	for (i = 0; i < NumberPhilosophers; ++i) {
		(args+i)->n = i;
		(args+i)->fork_l = forks+i;
		(args+i)->fork_r = forks+(i+1)%NumberPhilosophers;
		(args+i)->glutton_sem = &sem;
		(args+i)->hunger = (hunger+i);
	}

	NumberGluttons = NumberPhilosophers/2;
    // Создание потоков
	for (i = 0; i < NumberPhilosophers; i++) {
        if (pthread_create(threads+i, NULL, (i % 2 == 0 && i/2 < NumberGluttons) ? glutton_eat : eat, args+i) != 0) {
            fprintf(stderr, "Failed to create \"Philosopher\".\n");
            return 1;
        }
		if (i == 0) sem_wait(&sem);
    }

	while(1) {
		sleep(5);
		printf("\nHunger\n");
		for (i = 0; i < NumberPhilosophers; ++i) {
			printf(" %d ", hunger[i]);
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
		pthread_mutex_destroy((forks+i));
	}	

	free(forks);
	free(args);
	free(threads);
	free(hunger);

    return 0;
}

