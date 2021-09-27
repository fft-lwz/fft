#include "kiss_fftr.h"
#include "fftw3.h"
#include	<sndfile.h>
#include "kiss_fft.h"

void fft_d(double * in, fftw_complex* out, int nfft) {
	fftw_plan p;
	//p = fftw_plan_dft_1d(nfft, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	// 一维实数据的DFT
	p = fftw_plan_dft_r2c_1d(nfft, in, out, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
}

void ifft_d(fftw_complex* in, double* out, int nfft) {
	fftw_plan p;
	//p = fftw_plan_dft_1d(nfft, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	p = fftw_plan_dft_c2r_1d(nfft, in, out, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
}

void testfft() {
	int method = 1;
	//const int N = 11125;
	SF_INFO sf_info;
	SNDFILE *snd_file;
	snd_file = sf_open("p232_084_8kHz.wav", SFM_READ, &sf_info);

	float buf2[M];
	//buf2 = (double *)malloc(sf_info.frames * sizeof(double));
	//sf_readf_double(snd_file, buf2, sf_info.frames);
	printf("Sample Rate : %d\n", sf_info.samplerate);
	printf("Channels : %d\n", sf_info.channels);
	printf("Sections  : %d\n", sf_info.sections);
	printf("Frames   : %d\n", (int)sf_info.frames);
	int n_frames = (int)sf_info.frames / M;

	SNDFILE	*output_file;
	SF_INFO outsf_info;

	if (!(output_file = sf_open("sine.wav", SFM_WRITE, &sf_info)))
	{
		printf("Error : Not able to open output file.\n");
		return 1;
	}
	if (method == 0) {//方法一使用kissfft
		kiss_fft_cpx cpx_in[M];
		kiss_fft_cpx cx_out[M];
		kiss_fft_cfg cfg = kiss_fft_alloc(M, 0, NULL, NULL);


		kiss_fftr_cfg icfg = kiss_fftr_alloc(M, 1, 0, 0);
		kiss_fft_cpx freq_data[M / 2 + 1];
		kiss_fft_scalar time_data[M];
		for (int n = 0; n < n_frames; n++) {
			sf_readf_float(snd_file, buf2, M);

			for (int i = 0; i < M; i++)
			{
				cpx_in[i].r = buf2[i];
				cpx_in[i].i = 0;
			}

			kiss_fft(cfg, cpx_in, cx_out);

			/* inverse FFT */
			for (int i = 0; i < M / 2 + 1; i++)
			{
				freq_data[i].r = cx_out[i].r;
				freq_data[i].i = cx_out[i].i;
			}
			kiss_fftri(icfg, freq_data, time_data);

			float write_data[M];
			for (int i = 0; i < M; i++)
			{
				write_data[i] = time_data[i] / M;
				if (i == 0) printf("%f %f %f\n", write_data[i], buf2[i], cpx_in[i].r);
			}
			sf_write_float(output_file, write_data, M);
		}
		free(cfg);
	}
	else {//方法二：使用FFTW
		double in[M];
		fftw_complex out[M*2];

		fftw_complex freq_data[M * 2];
		double time_data[M];

		for (int n = 0; n < n_frames; n++) {
			sf_readf_float(snd_file, buf2, M);
			for (int i = 0; i < M; i++)
			{
				in[i] = buf2[i];
			}
			fft_d(in, out, M);

			/* inverse FFT */
			for (int i = 0; i < M; i++)
			{
				freq_data[i][0] = out[i][0];
				freq_data[i][1] = out[i][1];
			}
			ifft_d(freq_data, time_data, M);

			float write_data[M];
			for (int i = 0; i < M; i++)
			{
				write_data[i] = time_data[i] / M;
				if (i == 0) printf("%f %f %f\n", write_data[i], buf2[i], in[i]);
			}
			sf_write_float(output_file, write_data, M);
		}
	}
	
	sf_close(output_file);
	sf_close(snd_file);
	
}


