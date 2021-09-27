#include "fft.c"

#define FFT_LEN 100

complex *fft_buff;

void measure_fft(float *phase, float *amplitude)
{
	int index=0, max_index=1;
	float max_value, lx, ly;
    for(index = 0; index < FFT_LEN; index++)
    {
    	fft_buff[index].real = input[index];
    	fft_buff[index].imag = 0;
    } 
    fft(FFT_LEN, fft_buff);		//进行FFT处理

    //从1开始，去掉直流分量
    max_value=c_value(fft_buff[max_index]);
    for(index = 1; index < FFT_LEN; index++)
    {
    	if(c_value(fft_buff[index])>max_value)
    	{
    		max_index = index;
    		max_value = c_value(fft_buff[index]);
    	}
    }
    if(max_index!=1)
    {
    	printf("error! fft freq wrong!\r\n"); 
    }

    lx = fft_buff[max_index].real ;
    ly = fft_buff[max_index].imag ;
    *phase = atanf(lx / ly);
    *amplitude = sqrt(max_value);
}

int main() {
	return 0;
}