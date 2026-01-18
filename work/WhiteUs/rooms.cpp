#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define START_PLAYERS 5
#define PROBABILITY 6 // Шанс выстрела 1 к 6

// Глобальные переменные
int living_count = START_PLAYERS;

// Переменные для синхронизации
pthread_mutex_t m;

// Барьер 1: Зеленая дверь (Комната 1 -> Комната 2)
pthread_cond_t cond_green;
int waiting_green = 0;

// Барьер 2: Красная дверь (Комната 2 -> Комната 1)
pthread_cond_t cond_red;
int waiting_red = 0;

// Флаг окончания игры
int game_over = 0;

void* player(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // --- КОМНАТА 1 ---
        pthread_mutex_lock(&m);
        
        // Проверка победы перед началом раунда
        if (living_count <= 1) {
            game_over = 1;
            // Будим всех, чтобы они тоже вышли из циклов и завершились
            pthread_cond_broadcast(&cond_green);
            pthread_cond_broadcast(&cond_red);
            pthread_mutex_unlock(&m);
            break;
        }

        // Подходим к Зеленой двери
        waiting_green++;
        
        if (waiting_green == living_count) {
            // Я последний!
            printf("\033[1;32m[ЗЕЛЕНАЯ ДВЕРЬ] Игрок %d (последний) открывает дверь для %d чел.\033[0m\n", id, living_count);
            // Сбрасываем счетчик для следующего цикла (важно!)
            waiting_green = 0; 
            // Будим всех
            pthread_cond_broadcast(&cond_green);
        } else {
            // Я не последний, жду
            printf("Игрок %d ждет у Зеленой двери...\n", id);
            // Цикл while нужен для защиты от ложных пробуждений, 
            // но здесь условие выхода - просто сигнал broadcast, 
            // так как waiting_green сбрасывается последним.
            pthread_cond_wait(&cond_green, &m);
        }
        
        // Если игра закончилась пока мы спали
        if (game_over) {
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_mutex_unlock(&m);

        // --- ПЕРЕХОД В КОМНАТУ 2 ---
        printf("Игрок %d зашел в Комнату 2.\n", id);
        usleep(rand() % 500000 + 200000); // Имитация ходьбы

        // --- РУССКАЯ РУЛЕТКА ---
        int bullet = rand() % PROBABILITY;
        int dead = (bullet == 0);

        pthread_mutex_lock(&m);
        
        if (dead) {
            printf("\033[1;31m[ВЫСТРЕЛ] Игрок %d: BANG! (Умер)\033[0m\n", id);
            living_count--;

            // КРИТИЧЕСКИЙ МОМЕНТ:
            // Если я умер, я больше не участвую в барьерах.
            // Но возможно, выжившие уже стоят у Красной двери и ждут МЕНЯ (или количества, включающего меня).
            // Так как я умер, living_count уменьшился.
            // Нужно проверить: не набралось ли теперь достаточно выживших у двери?
            
            if (waiting_red >= living_count && living_count > 0) {
                printf("\033[1;31m(Призрак %d открывает Красную дверь, так как умер последним)\033[0m\n", id);
                waiting_red = 0;
                pthread_cond_broadcast(&cond_red);
            }
            
            // Если умерли все или остался 1 - конец игры
            if (living_count <= 1) {
                game_over = 1;
                pthread_cond_broadcast(&cond_green); // Будим всех везде
                pthread_cond_broadcast(&cond_red);
            }

            pthread_mutex_unlock(&m);
            return NULL; // Выход из потока (смерть)
        } else {
            printf("Игрок %d: CLICK (Жив). Идет к Красной двери.\n", id);
            
            // --- КРАСНАЯ ДВЕРЬ ---
            waiting_red++;
            
            if (waiting_red == living_count) {
                // Я последний выживший в этом раунде
                printf("\033[1;31m[КРАСНАЯ ДВЕРЬ] Игрок %d отстрелялся последним. Открывает дверь.\033[0m\n", id);
                waiting_red = 0;
                pthread_cond_broadcast(&cond_red);
            } else {
                // Жду остальных выживших
                pthread_cond_wait(&cond_red, &m);
            }
        }
        
        if (game_over) {
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_mutex_unlock(&m);
        
        // Возврат в комнату 1
        printf("Игрок %d вернулся в Комнату 1.\n", id);
    }
    
    printf("\033[1;33mИгрок %d: Я ПОБЕДИЛ (или выжил)!\033[0m\n", id);
    return NULL;
}

int main() {
    pthread_t threads[START_PLAYERS];
    int ids[START_PLAYERS];
    srand(time(NULL));

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cond_green, NULL);
    pthread_cond_init(&cond_red, NULL);

    printf("=== ИГРА НАЧАЛАСЬ: 2 КОМНАТЫ, %d ИГРОКОВ ===\n", START_PLAYERS);

    for (int i = 0; i < START_PLAYERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, player, &ids[i]);
    }

    for (int i = 0; i < START_PLAYERS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n=== ИГРА ОКОНЧЕНА ===\n");
    if (living_count == 1) printf("Есть победитель!\n");
    else if (living_count == 0) printf("Все умерли. Ничья.\n");

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cond_green);
    pthread_cond_destroy(&cond_red);

    return 0;
}