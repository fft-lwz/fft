# include <time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "fftw3.h"

// 本程序仅考虑了单线程情况
int main(int argc, char *argv[]) {
    int n_array;
    fftw_complex *in, *out;
    fftw_plan p;
    char* flags;

    if (argc < 3) {
        printf("Usage: ./fftw_test [N_ARRAY] [TYPE].\n");
    } else {
        n_array = atoi(argv[1]);
        flags = argv[2];
    }

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_array);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_array);

    clock_t t1 = clock();
    p = fftw_plan_dft_1d(n_array, in, out, FFTW_FORWARD, strcmp(argv[2], "ESTIMATE") == 0 ? FFTW_ESTIMATE : FFTW_MEASURE);
    clock_t t2 = clock();
    fftw_execute(p);
    clock_t t3 = clock();

    printf("plan time: %f ms, exec time: %f ms.\n", (t2 - t1) / 1000.0, (t3 - t2) / 1000.0);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return 0;
}
