#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 100

// Структура для передачи аргументов в поток
typedef struct {
    int* buns;
    sem_t* semaphore;
	int* counter;
	int* goal;
	int* made;
} ThreadArgsGeneral;

typedef struct {
	ThreadArgsGeneral* GeneralArgs;
	int time;
} ThreadArgs;


// Функция, которую выполняет каждый пекарь
void* backer(void* arg) {
    ThreadArgs args; ThreadArgsGeneral* GenArgs; int time = 0;

   	args = *(ThreadArgs*)arg;

	GenArgs = args.GeneralArgs;
	time = args.time;

	while(1) {
		sleep(time);
		sem_wait(GenArgs->semaphore);
		if (*GenArgs->made == *GenArgs->goal) {
			return NULL;
		}
		if (*GenArgs->buns < *GenArgs->counter) {
			++(*GenArgs->buns);
			++(*GenArgs->made);
			sem_post(GenArgs->semaphore);
			sleep(time);
		}
		else sem_post(GenArgs->semaphore);

	}
	return NULL;
}

// Функция, которую выполняет каждый покупатель
void* customer(void* arg){
	ThreadArgs args; ThreadArgsGeneral* GenArgs; int time = 0;

   	args = *(ThreadArgs*)arg;

	GenArgs = args.GeneralArgs;
	time = args.time;

	while(1) {
		sleep(time);
		sem_wait(GenArgs->semaphore);
		if (*GenArgs->buns > 0) {
			--(*GenArgs->buns);
			sem_post(GenArgs->semaphore);
			sleep(time);
		}
		else {
			if (*GenArgs->made == *GenArgs->goal) {sem_post(GenArgs->semaphore); return NULL;}
			sem_post(GenArgs->semaphore);
		}

	}
	return NULL;

}

int main(int argc, char* argv[]) {
	int NumberBackers = 0, NumberCustomers = 0, counter = 0, n = 0, buns = 0, goal = 0, i = 0, made = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    NumberBackers = atoi(argv[1]);
	NumberCustomers = atoi(argv[2]);
	counter = atoi(argv[3]);
	n = NumberBackers + NumberCustomers;

	if (NumberBackers <= 0 || NumberCustomers <= 0 || counter <= 0 || n > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads or size of counter. Must be between 1 and %d.\n", MAX_THREADS);
        return 1;
    }

	goal = 2*n;

    srand(time(NULL));
    pthread_t threads[n];
	sem_t semaphore;

    // Инициализация семафора
    if (sem_init(&semaphore, 0, 1) != 0) {
        perror("Failed to initialize semaphore");
        return 1;
    }

    // Аргументы для потоков
    ThreadArgsGeneral GeneralArgs = {&buns, &semaphore, &counter, &goal, &made};
	ThreadArgs args = {&GeneralArgs, 0};

    // Создание потоков
	for (i = 0; i < NumberBackers; i++) {
		args.time = (rand() % 301 )/100. + 1;
        if (pthread_create(&threads[i], NULL, backer, &args) != 0) {
            perror("Failed to create \"Backer\".\n");
            return 1;
        }
    }

    for (i = 0; i < NumberCustomers; i++) {
		args.time = (rand() % 301 )/100. + 1;
        if (pthread_create(&threads[i+NumberBackers], NULL, customer, &args) != 0) {
            perror("Failed to create \"Customer\".\n");
            return 1;
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    // Уничтожение семафора
    sem_destroy(&semaphore);

    printf("Goal achieved: %d/%d\n", made, goal);
    return 0;
}

