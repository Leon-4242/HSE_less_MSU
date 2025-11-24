#!/usr/bin/env python3
import random
import time

def main():
    interval = 1 / 10
    while True:
        # Обновление данных таблицы (пример)
        table = [[random.randrange(10) for _ in range(6)] for _ in range(3)]

        # Очистка экрана. \033c – это ANSI escape-последовательность для очистки экрана
        print('\033c', end='')

        # Печать обновленной таблицы
        for row in table:
            print(' '.join(map(str, row)))
        
        # Пауза перед следующим обновлением
        time.sleep(interval)

if __name__ == '__main__':
    main()

