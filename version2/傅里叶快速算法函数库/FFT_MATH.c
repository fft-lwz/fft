#include "fft_math.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#define PI 3.1415926535


//复数FFT 
void FFT(float dataR[],float dataI[],float dataA[],int N,int M)
{

	int i,j,k,r;
	int p,L,B;
	unsigned int I,J,K,F0,F1,m,n;
	float Tr,Ti,temp;
	//输入序列倒序
/*
J=N/2;
for(I = 1; I < N - 2; I++)
{
	//数组元素互换
	if(I<J)
	{
		temp = dataR[I];
		dataR[I] = dataR[J];
		dataR[J] = temp;
	}
	//求取下一个反序数
	K=N/2;//K是N的权值变量，从最高位权值开始判断。
	while(1)
	{
		//从左到右循环判断该位是否为1
 		if(J< K)	
		{
			J=J+K;//判断结果为该位为0，加上该位权值，0变成1。
			break;//循环结束
 		}
		else 
		{
			J=J-K;//判断结果为该位为1，减去该位权值，1变成0。
			K=K/2;//得到下一位的权值，继续下一位0或1的判断。
		 }
	}
}*/

//倒序
	for(I=0;I< N;I++)   //根据规律四，需要对数组元素执行码间倒序
	{  
		/*获取下标I的反序J的数值*/ 
		J=0;
		for(k=0;k<(M/2+0.5);k++)     //k表示操作
	     {
	         //*反序操作*/
	         m=1;//m是最低位为1的二进制数
			 n=(unsigned int)pow(2,M-1);//n是第M位为1的二进制数
			 m <<= k; //对m左移k位
	         n >>= k; //对n右移k位
	         F0=I & n;//I与n按位与提取出前半部分第k位
			 F1=I & m;//I与m按位与提取出F0对应的后半部分的低位
	         if(F0) J=J | m; //J与m按位或使F0对应低位为1
	         if(F1) J=J | n; //J与n按位或使F1对应高位为1 
	     }
	 		//printf("I为：");printBin(I,M);printf(" %d\n",I);
	 		//printf("J为：");printBin(J,M);printf(" %d\n\n",J);
	 		
	 	 if(I<J)
			{
				temp = dataR[I];
				dataR[I] = dataR[J];
				dataR[J] = temp;
				//补充虚数部分交换： 
				temp = dataI[I];
				dataI[I] = dataI[J];
				dataI[J] = temp;
			}                                
	} 

//进行FFT
	/*	Tr = Xr(J+B)cos(2.0*PI*p/N) + Xi(J+B)sin(2.0*PI*p/N)
	    Ti = Xi(J+B)cos(2.0*PI*p/N) - Xr(J+B)sin(2.0*PI*p/N)
		Ar[J] = Xr(J) + Tr
	    Ai[J] = Xi(J) + Ti
	    Ar[J+B] = Xr(J) - Tr
	    Ai[J+B] = Xi(J) - Ti
	   (其中 Xr为上一级的Ar, Xi为上一级的Ai)*/
	for(L=1; L<=M;L++)	//FFT蝶形级数L从1--M
	{		
		/*第L级的运算:
		然后对于第L级，我们在问题五种提到，蝶形运算的种类数目等于间隔B:有多少种蝶形运算就要需要循环多少次;
		随着循环的不同，旋转指数P也不同，P的增量为k=2^(M-L)*/	
		//先计算一下间隔 B = 2^(L-1);
		B = 1;
		B = (int)pow(2,L-1);	
		for(j=0;j<=B-1;j++)		
		//j = 0,1,2,...,2^(L-1) - 1
		{	/*同种蝶形运算*/	
			//先计算增量k=2^(M-L)
			k=1;
			k = (int)pow(2,M-L);
			//计算旋转指数p，增量为k时，则P=j*k
			p=1;
			p=j*k;
			/*接下来，由问题六我们可以知道同种蝶形运算的次数刚好为增量k=2^(M-L)；
			同种蝶形的运算次数即为蝶形运算的次数*/
			for(i=0; i<=k-1;i++) 
				{
					//数组下标定为r
					r=1;
					r=j+2*B*i;
					Tr=dataR[r+B]*cos(2.0*PI*p/N) + dataI[r+B]*sin(2.0*PI*p/N);
					Ti=dataI[r+B]*cos(2.0*PI*p/N) - dataR[r+B]*sin(2.0*PI*p/N);
					dataR[r+B]=dataR[r]-Tr;
					dataI[r+B]=dataI[r]-Ti;
					dataR[r]=dataR[r]+Tr;
					dataI[r]=dataI[r]+Ti;
				}	
			}
	}
	//计算幅值 
	for ( i=0;i<N;i++ )
	{ 		
	 	dataA[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
	}
}

//实数FFT 
void FFTR(float dataR[],float dataI[],float dataA[],int N,int M)
{
	int i,k;
	int j,r,p,B;
	float Tr,Ti;
	float yR[N/2],yI[N/2],yA[N/2]; 
	float x1R[N/2],x2R[N/2],x1I[N/2],x2I[N/2],xR[N],xI[N];
	for(i=0;i<N/2;i++)
	{
		yR[i]=dataR[2*i];
		yI[i]=dataR[2*i+1];
				
	}
	FFT(yR,yI,yA,N/2,M-1);
	//求X1(k)和X2(k) 
	for(k=0;k<N/2;k++)
	{
		if(k==0)
		{
			x1R[k]=yR[k];
			x1I[k]=yI[k];
			x2R[k]=yI[k];
			x2I[k]=-yR[k];	
		}
		else
		{
			x1R[k]=(yR[k]+yR[N/2-k])/2;
			x1I[k]=(yI[k]-yI[N/2-k])/2;
			x2R[k]=(yI[k]+yI[N/2-k])/2;
			x2I[k]=(yR[N/2-k]-yR[k])/2;
		}
	}
	
	//第M级的蝶形运算		
	B = 1;
	B = (int)pow(2,M-1);	
	for(j=0;j<=N/2-1;j++)		
	//j = 0,1,2,...,2^(L-1) - 1
	{		
		p=1;
		p=j;
		//数组下标定为r
		r=1;
		r=j;
		int k=j;
		Tr=x2R[r]*cos(2.0*PI*p/N) + x2I[r]*sin(2.0*PI*p/N);
		Ti=x2I[r]*cos(2.0*PI*p/N) - x2R[r]*sin(2.0*PI*p/N);
		xR[r]=x1R[r]+Tr;
		xI[r]=x1I[r]+Ti;

		if(r==0)
		{
			xR[N/2]=x1R[0]-x2R[0];
			xI[N/2]=x1I[0]-x2I[0];		
		}
		else
		{
			xR[N-r]=xR[r];
			xI[N-r]=-xI[r];	
		}
	}
	
	for(i=0;i<N;i++)
	{
		dataR[i]=xR[i];
		dataI[i]=xI[i];
	}
	for ( i=0;i<N;i++ )
	{ 		
		//dataA[i]=sqrt(xR[i]*xR[i]+xI[i]*xI[i]);
	 	dataA[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
	}	
} 


//复数IFFT 
void IFFT(float dataR[],float dataI[],float dataA[],int N,int M) 
{
	int i,j,k,r;
	int p,L,B;
	unsigned int I,J,K,F0,F1,m,n;
	float Tr,Ti,temp;
	//输入序列倒序
/*
J=N/2;
for(I = 1; I < N - 2; I++)
{
	//数组元素互换
	if(I<J)
	{
		temp = dataR[I];
		dataR[I] = dataR[J];
		dataR[J] = temp;
		//补充虚数部分交换： 
		temp = dataI[i];
		dataI[I] = dataI[J];
		dataI[J] = temp;
	}
	//求取下一个反序数
	K=N/2;//K是N的权值变量，从最高位权值开始判断。
	while(1)
	{
		//从左到右循环判断该位是否为1
 		if(J< K)	
		{
			J=J+K;//判断结果为该位为0，加上该位权值，0变成1。
			break;//循环结束
 		}
		else 
		{
			J=J-K;//判断结果为该位为1，减去该位权值，1变成0。
			K=K/2;//得到下一位的权值，继续下一位0或1的判断。
		 }
	}
}*/
	
	//倒序
	for(I=0;I< N;I++)   //根据规律四，需要对数组元素执行码间倒序
	{  
		/*获取下标I的反序J的数值*/ 
		J=0;
		for(k=0;k<(M/2+0.5);k++)     //k表示操作
	     {
	         //*反序操作*/
	         m=1;//m是最低位为1的二进制数
			 n=(unsigned int)pow(2,M-1);//n是第M位为1的二进制数
			 m <<= k; //对m左移k位
	         n >>= k; //对n右移k位
	         F0=I & n;//I与n按位与提取出前半部分第k位
			 F1=I & m;//I与m按位与提取出F0对应的后半部分的低位
	         if(F0) J=J | m; //J与m按位或使F0对应低位为1
	         if(F1) J=J | n; //J与n按位或使F1对应高位为1 
	     }
	 		//printf("I为：");printBin(I,M);printf(" %d\n",I);
	 		//printf("J为：");printBin(J,M);printf(" %d\n\n",J);
	 		
	 	 if(I<J)
			{
			temp = dataR[I];
			dataR[I] = dataR[J];
			dataR[J] = temp;
			//补充虚数部分交换： 
			temp = dataI[I];
			dataI[I] = dataI[J];
			dataI[J] = temp;
			}                                
	} 

//进行IFFT
	/*	Tr = Xr(J+B)cos(2.0*PI*p/N) - Xi(J+B)sin(2.0*PI*p/N)
	    Ti = Xi(J+B)cos(2.0*PI*p/N) + Xr(J+B)sin(2.0*PI*p/N)
		Ar[J] = Xr(J) + Tr
	    Ai[J] = Xi(J) + Ti
	    Ar[J+B] = Xr(J) - Tr
	    Ai[J+B] = Xi(J) - Ti
	   (其中 Xr为上一级的Ar, Xi为上一级的Ai)*/
	for(L=1; L<=M;L++)	//FFT蝶形级数L从1--M
	{		
		/*第L级的运算:
		然后对于第L级，我们在问题五种提到，蝶形运算的种类数目等于间隔B:有多少种蝶形运算就要需要循环多少次;
		随着循环的不同，旋转指数P也不同，P的增量为k=2^(M-L)*/	
		//先计算一下间隔 B = 2^(L-1);
		B = 1;
		B = (int)pow(2,L-1);	
		for(j=0;j<=B-1;j++)		
		//j = 0,1,2,...,2^(L-1) - 1
		{	/*同种蝶形运算*/	
			//先计算增量k=2^(M-L)
			k=1;
			k = (int)pow(2,M-L);
			//计算旋转指数p，增量为k时，则P=j*k
			p=1;
			p=j*k;
			/*接下来，由问题六我们可以知道同种蝶形运算的次数刚好为增量k=2^(M-L)；
			同种蝶形的运算次数即为蝶形运算的次数*/
			for(i=0; i<=k-1;i++) 
				{
					//数组下标定为r
					r=1;
					r=j+2*B*i;
					Tr=dataR[r+B]*cos(2.0*PI*p/N) - dataI[r+B]*sin(2.0*PI*p/N);
					Ti=dataI[r+B]*cos(2.0*PI*p/N) + dataR[r+B]*sin(2.0*PI*p/N);
					dataR[r+B]=dataR[r]-Tr;dataR[r+B]=dataR[r+B]/2; 
					dataI[r+B]=dataI[r]-Ti;dataI[r+B]=dataI[r+B]/2;
					dataR[r]=dataR[r]+Tr;dataR[r]=dataR[r]/2;
					dataI[r]=dataI[r]+Ti;dataI[r]=dataI[r]/2;
				}	
			}
	}
	//求出幅度频率谱,dataA[i] = sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i])
	for ( i=0;i<N;i++ )
	{ 		
	 	dataA[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
	}
}

//实数IFFT
void IFFTR(float dataR[],float dataI[],float dataA[],int N,int M)
{
	int i,k;
	int j,r,p,B;
	float Tr,Ti;
	float yR[N/2],yI[N/2],yA[N/2]; 
	float x1R[N/2],x2R[N/2],x1I[N/2],x2I[N/2],xR[N],xI[N];
	for(i=0;i<N/2;i++)
	{
		yR[i]=dataR[2*i];//printf("%f\n",yR[i]);
		yI[i]=dataR[2*i+1];//printf("%f\n",yI[i]);
				
	}
	IFFT(yR,yI,yA,N/2,M-1);
	for(k=0;k<N/2;k++)
	{
		printf("%d,%f\n",N/2,yR[k]);
		//printf("%f\n",yI[N/2]);
	}
	//求X1(k)和X2(k) 
	for(k=0;k<N/2;k++)
	{
		if(k==0)
		{
			x1R[k]=yR[k];
			x1I[k]=yI[k];
			x2R[k]=yI[k];
			x2I[k]=-yR[k];	
		}
		else
		{
			x1R[k]=(yR[k]+yR[N/2-k])/2;
			x1I[k]=(yI[k]-yI[N/2-k])/2;
			x2R[k]=(yI[k]+yI[N/2-k])/2;
			x2I[k]=(yR[N/2-k]-yR[k])/2;
		}
	
	}
	
	//第M级的蝶形运算		
	B = 1;
	B = (int)pow(2,M-1);	
	for(j=0;j<=B-1;j++)		
	//j = 0,1,2,...,2^(L-1) - 1
	{		
		p=1;
		p=j;
		//数组下标定为r
		r=1;
		r=j;
		Tr=x2R[r]*cos(2.0*PI*p/N) - x2I[r]*sin(2.0*PI*p/N);
		Ti=x2I[r]*cos(2.0*PI*p/N) + x2R[r]*sin(2.0*PI*p/N);
		dataR[r]=x1R[r]+Tr;dataR[r]=dataR[r]/2;
		dataI[r]=x1I[r]+Ti;dataI[r]=dataI[r]/2;

		if(r==0)
		{
			dataR[N/2]=x1R[0]-x2R[0];dataR[N/2]=dataR[N/2]/2;
			dataI[N/2]=x1I[0]-x2I[0];dataI[N/2]=dataI[N/2]/2;	
		}
		else
		{
			dataR[N-r]=dataR[r];
			dataI[N-r]=-dataI[r];	
		}
	}
	

	for ( i=0;i<N;i++ )
	{ 		
		//dataA[i]=sqrt(xR[i]*xR[i]+xI[i]*xI[i]);
	 	dataA[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
	}	
}  
