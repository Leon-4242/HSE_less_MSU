#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 100


// Структура для передачи аргументов в поток
typedef struct {
    sem_t* sem_b;
	sem_t* sem_c;
	int *goal;
} ThreadArgs;

void* backer(void*);
void* customer(void*);

// Функция, которую выполняет каждый пекарь
void* backer(void* arg) {
    ThreadArgs* args;  int time = 0, made = 0, goal = 0;

   	args = (ThreadArgs*)arg;
	goal = *args->goal;
	time = (rand() % 5)+1;

	while(1) {
		sleep(time);
		if (made >= goal) break;
		sem_wait(args->sem_b);
		sem_post(args->sem_c);
		++made;
	}
	printf("\n I made %d buns.\n", goal);
	return NULL;
}

// Функция, которую выполняет каждый покупатель
void* customer(void* arg){
    ThreadArgs* args;  int time = 0, bought = 0, goal = 0;

   	args = (ThreadArgs*)arg;
	goal = *args->goal;
	time = (rand() % 5)+1;

	while(1) {
		sleep(time);
		if (bought >= goal) break;
		sem_wait(args->sem_c);
		sem_post(args->sem_b);
		++bought;
	}

	printf("\n I bought %d buns.\n", goal);
	return NULL;
}

int main(int argc, char* argv[]) {
	int NumberBackers = 0, NumberCustomers = 0, counter = 0, n = 0, i = 0, goal = 0;
	pthread_t* threads; sem_t SemBunsCustomer, SemBunsBacker; ThreadArgs args;

    if (argc != 4) {
        fprintf(stderr, "Wrong arguments.\nNeed: NumberBackers NumberCustomers counter\n");
        return 1;
    }
	srand(time(NULL));

    NumberBackers = atoi(argv[1]);
	NumberCustomers = atoi(argv[2]);
	counter = atoi(argv[3]);
	n = NumberBackers + NumberCustomers;

	if (NumberBackers <= 0 || NumberCustomers <= 0 || counter <= 0 || n > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads or size of counter. Must be between 1 and %d.\n", MAX_THREADS);
        return 1;
    }

	threads = (pthread_t*)malloc(n*sizeof(pthread_t));

    // Инициализация семафора
    if (sem_init(&SemBunsCustomer, 0, 0) != 0 || sem_init(&SemBunsBacker, 0, counter) != 0) {
        fprintf(stderr, "Failed to initialize semaphore");
        return 1;
    }

	goal = counter;

    // Аргументы для потоков
    args.sem_b = &SemBunsBacker;
	args.sem_c = &SemBunsCustomer;
	args.goal =	&goal;

    // Создание потоков
	for (i = 0; i < NumberBackers; i++) {
        if (pthread_create(threads+i, NULL, backer, &args) != 0) {
            fprintf(stderr, "Failed to create \"Backer\".\n");
            return 1;
        }
    }

    for (i = 0; i < NumberCustomers; i++) {
        if (pthread_create(threads+NumberBackers+i, NULL, customer, &args) != 0) {
            fprintf(stderr, "Failed to create \"Customer\".\n");
            return 1;
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread");
            return 1;
        }
    }

    // Уничтожение семафора
    sem_destroy(&SemBunsCustomer);
	sem_destroy(&SemBunsBacker);
	
	free(threads);
    return 0;
}

