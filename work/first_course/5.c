#include <stdio.h>

int main(void) {
        FILE *fin, *fout;
       	int res = 0, curr = 0;
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
		fprintf(fout, "1");
		fclose(fin);
                fclose(fout);
                return 0;
        }
	
        while (fscanf(fin, "%d", &curr) == 1) {
                if (curr) {
			res += curr;
			if (res < 0) {
				fprintf(fout, "0");
				fclose(fin);
				fclose(fout);
				return 0;
			}
		}
        }

        fprintf(fout, "%d", (res) ? 0 : 1);
	fclose(fin);
	fclose(fout);
        return 0;
}

