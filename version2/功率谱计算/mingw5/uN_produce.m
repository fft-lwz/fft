%%
clc;
clear;
N=128;
%����һ�γ���ΪN����ֵΪ0������Ϊ1�ĸ�˹����������
v=randn(1,N);
%��������ΪN������ʵ�������У�����Ƶ�ʦطֱ�Ϊ0.1��0.25��0.27������ȷֱ�Ϊ30dB��30dB��27dB��
i=1:N;
u1=sin(2*pi*0.1*i);u2=sin(2*pi*0.25*i);u3=sin(2*pi*0.27*i);
U1=awgn(u1,30);U2=awgn(u2,30);U3=awgn(u3,27);
%����ʵ�������к�ʵ��˹���������н��е��ӵõ��۲��ź�uN(n)
uN=v+U1+U2+U3;
uN=uN';
subplot(2,2,1);plot(i,uN);title('ԭ�ź� N=128');
% fid = fopen('uN.txt','w');
% fprintf(fid,'%f\n',uN);       %n ����
% fclose(fid);
save uN.txt -ascii uN;
%%
uN=fft(uN,N);
uN=abs(uN);
uN=uN.*uN;
subplot(2,2,2);plot(i,uN);title('matlab power spectrum')
var2=load('output.txt');
subplot(2,2,4);plot(i,var2);title('C power spectrum');