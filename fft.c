#include <fftw3.h>

int main() {
        fftw_complex *in, *out;
        fftw_plan p;
        int N;
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);



        // 输入数据in赋值


        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p); // 执行变换
        fftw_destroy_plan(p);
        fftw_free(in); 
        fftw_free(out);
    }