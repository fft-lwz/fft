#include <stdio.h>
#include <fft_math.h>
//#include "fft_math.c"
#include <errno.h> 
#include <string.h>

int main(int argc, char *argv[])
{
	int N=128,M=7;
	float uN[N],P[N];
	int i=0;
	FILE *fp;
	//读取Matlab生成的观测信号uN 
	if((fp=fopen("uN.txt","r"))==NULL)
	{
 		printf("无法打开文件，错误是：%d-%s",errno,strerror(errno));
 		exit(0);//终止程序 
	}
		while(!feof(fp))
	{
	    fscanf(fp,"%f",&uN[i]);
	    P[i]=0;
	    i++;
	}
	fclose(fp);
	FFTR(uN,P,uN,N, M);//调用FFTR函数计算uN的傅里叶变换 
	//写入uN的幅度谱 
	if((fp=fopen("output.txt","wb"))==NULL)
	{
 		printf("无法打开文件");
 		exit(0);//终止程序 
	}
	for(i = 0; i < N ; i++) 	
	{
		P[i] = uN[i]*uN[i];
		fprintf(fp,"%f\n",P[i]);
	} 
	fclose(fp);	
}
