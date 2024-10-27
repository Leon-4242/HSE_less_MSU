#include <stdio.h>

int main(void) {
        FILE *fin, *fout;
        int count = 0, res = 0, curr = 0;
        fin = fopen("input.txt", "r");
        if (fin == NULL) {
                return -1;
        }
        fout = fopen("output.txt", "w");
        if (fout == NULL) {
                fclose(fin);
                return -1;
        }

        if (feof(fin)) {
		fclose(fin);
                fclose(fout);
                return 0;
        }

        while (fscanf(fin, "%d", &curr) == 1) {
                if (!count) {
			res = curr;
			count++;
		} else if (curr != res) {
			count--;
		} else {
			count++;
		}
        }

        fprintf(fout, "%d", res);
        return 0;
}

