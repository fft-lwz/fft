%SinIn[i] = A0 * (sin(2*PI*i/25)+sin(2*PI* i * 0.4 ));
clc
clear
N=128,A0=255;PI=3.1415926;
i=1:N;
x=A0 * (sin(2*PI*i/25)+sin(2*PI* i * 0.4 ));
y=fft(x,N);%���и���Ҷ�任��÷�����
z=abs(y);%����õķ����׼Ӿ���ֵ   
subplot(2,2,1);plot(i,x);title('MATLAB�е����벨��');
subplot(2,2,2);plot(i,z);title('MATLAB�ķ�����')
var1=load('input.txt');%��ȡC������N����������
var2=load('output.txt');%��ȡC������N��FFT��������
subplot(2,2,3);plot(i,var1);title('C�е����벨��');
subplot(2,2,4);plot(i,abs(var2));title('C�ķ�����');


