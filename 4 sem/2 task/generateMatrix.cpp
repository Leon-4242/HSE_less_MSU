#include <cstdio>
#include <random>

int main() {
    printf("Generating a Random Matrix\n");
    int rows, cols;
    printf("Enter the number of matrix rows and columns: ");
    if (scanf("%d%d", &rows, &cols) < 2 || rows <= 0 || cols <= 0) {
        printf("Incorrect matrix dimensions\n");
        return (-1);
    }
    double minValue, maxValue;
    printf("Enter the range of matrix elements (minValue, maxValue): ");
    if (
        scanf("%lg%lg", &minValue, &maxValue) < 2 || 
        maxValue - minValue <= 0
    ) {
        printf("Incorrect range of matrix elements\n");
        return (-1);
    }

    std::random_device rd;      // Seed source for the random generator
    std::mt19937 gen(rd());     // Create random generator
    std::uniform_real_distribution<double> randomValue(minValue, maxValue);

    double *a = new double[rows*cols];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            a[i*cols + j] = randomValue(gen);
            if (j > 0)
                printf(" ");
            printf("%.3f", a[i*cols + j]);
        }
        printf("\n");
    }

    // Write a matrix to the file
    char fileName[64];
    printf("Enter the name of file to write a matrix in:\n");
    scanf("%62s", fileName);
    fileName[62] = 0;
    FILE *f = fopen(fileName, "wt");
    int res = 0;
    if (f == NULL) {
        perror("Cannot open a file for writing");
        res = (-1);
    } else {
        printf("Writing the random matrix to the file %s\n", fileName);
        fprintf(f, "%d %d\n", rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (j > 0)
                    fprintf(f, " ");
                fprintf(f, "%.3f", a[i*cols + j]);
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }

    delete[] a;
    return res;
}

