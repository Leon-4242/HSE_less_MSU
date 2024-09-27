                int sum = 0;
                for (int i = 1; i < n-1; ++i) {
                        sum = 0;
                        for (int k = 1; k < num1.getN()-1; ++k) {
                                if ( 1 <= num1.getN()-k && num1.getN()-k < num2.getN()-1) {
                                        sum += str1[k]*str2[num1.getN()-k];
                                }
                        }
                        tmp[i] = sum;
                }

                delete[] str1; delete[] str2;

                CintN res(tmp);
                delete[] tmp;

                return res;

