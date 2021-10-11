# include <stdio.h>
# include <math.h>
# include <complex>
# include <random>
# include <stdlib.h>
# include <sys/time.h>
# include <omp.h>
using namespace std;
//# define PRINT

void FFT(int n, int f, complex <double> *arr) {
    // 交换数据
    int *r = new int[n];
    r[0] = 0;
    for (int i = 0, len = (int)log2(n) - 1; i < n; i++) {
        r[i] = (r[i >> 1] >> 1) | ((i & 1) << len);
        if (i < r[i]) {
            swap(arr[i], arr[r[i]]);
        }
    }
    delete[] r;

    // 以非递归形式计算
    for (int i = 2; i <= n; i <<= 1) {
        // 计算 wn
        complex <double> wn(cos(2 * M_PI/i), -f * sin(2 * M_PI/i));
        # pragma omp parallel for num_threads(4) proc_bind(close)
        for (int j = 0; j < n; j += i) {
            complex <double> w(1, 0);
            for (int k = 0; k < i / 2; k++) {
                complex <double> x = arr[j + k], y = w * arr[j + k + i / 2];
                arr[j + k] = x + y, arr[j + k + i / 2] = x - y;
                w *= wn;
            }
        }
    }
}

int main(int argc, char* argvs[]) {
    int n;
    if (argc < 2) {
        printf("Usage: ./cooley-tukey [N_ARRAY]\n");
        exit(1);
    } else {
        n = atoi(argvs[1]);
    }
    complex <double> *arr = new complex <double>[1 << (int)ceil(log2(n))];
    srand(time(0));
    for (int i = 0; i < n; i++) {
		arr[i] = complex <double> (rand(), rand());
	}

    timeval start, end;
    gettimeofday(&start, NULL);
    n = 1 << (int)ceil(log2(n));
    FFT(n, 1, arr);
    gettimeofday(&end, NULL);
    printf("Time: %lf ms.\n", (1000000.0 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000);

# ifdef PRINT
	for (int i = 0; i < n; i++) {
		printf("%lf %lf\n", arr[i].real(), arr[i].imag());
	}
# endif

	delete[] arr;
    return 0;
}
