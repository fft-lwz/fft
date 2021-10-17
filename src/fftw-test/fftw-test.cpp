# include <sys/time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "fftw3.h"

int main(int argc, char *argv[]) {
    int n_array, n_thread;
    fftw_complex *in, *out;
    fftw_plan p;
    char* flags;

    if (argc < 4) {
        printf("Usage: ./fftw_test [N_ARRAY] [N_THREAD] [TYPE].\n");
        exit(1);
    } else {
        n_array = atoi(argv[1]);
        n_thread = atoi(argv[2]);
        flags = argv[2];
    }

    fftw_init_threads();
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_array);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_array);

    timeval start, end;
    gettimeofday(&start, NULL);
    fftw_plan_with_nthreads(n_thread);
    p = fftw_plan_dft_1d(n_array, in, out, FFTW_FORWARD, strcmp(argv[2], "ESTIMATE") == 0 ? FFTW_ESTIMATE : FFTW_MEASURE);
    gettimeofday(&end, NULL);
    printf("Plan Time: %lf ms.\n", (1000000.0 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000);

    gettimeofday(&start, NULL);
    fftw_execute(p);
    gettimeofday(&end, NULL);
    printf("Exec Time: %lf ms.\n", (1000000.0 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000);
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    fftw_cleanup_threads();

    return 0;
}
