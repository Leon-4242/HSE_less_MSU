#include <stdio.h>
#include <unistd.h>

#define WIDTH 50

int main() {
    for (int i = 0; i <= 100; i++) {
        int pos = i * WIDTH / 100;

        printf("\r[");
        for (int j = 0; j < WIDTH; j++) {
            if (j < pos) printf("=");
            else if (j == pos) printf(">");
            else printf(" ");
        }
        printf("] %d%%", i);

        fflush(stdout);
        usleep(50000);
    }
    printf("\n");
}
