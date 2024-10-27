#include <stdio.h>

int main(void) {
        FILE *fin, *fout;
        int max = 0, res = 0, curr = 0;
        fin = fopen("input.txt", "r");
        if (fin == NULL) {
                return -1;
        }
        fout = fopen("output.txt", "w");
        if (fout == NULL) {
                fclose(fin);
                return -1;
        }

	if (fscanf(fin, "%d", &max) < 1) {
		fclose(fin);
		fclose(fout);
		return 0;
	}
        while (fscanf(fin, "%d", &curr) == 1) {
                if (curr > max) {
			max = curr;
		}
        }
	rewind(fin);
	
	while (fscanf(fin, "%d", &curr) == 1) {
		if (curr == max) continue; else break;
	}
	res = curr;
	while (fscanf(fin, "%d", &curr) == 1) {
		if (curr == max) continue;
		if (curr > res) {
			res = curr;
		}
	}
	if (res == max) res = 0;

        fprintf(fout, "%d", res);

	fclose(fin);
	fclose(fout);
        return 0;
}
