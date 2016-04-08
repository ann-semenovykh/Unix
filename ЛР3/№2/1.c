#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

const int N=100;
void main(int argc,char *argv[])
{
	FILE *f;
	f=fopen("100by100.txt","w");
	int i,j;
	int matrix[N][N];
	int vect[N];
	int res[N];
	for (i=0;i<N+1;i++)
	{
		for (j=0;j<N;j++) 
		if(i!=N)
			{matrix[i][j]=random()%10;
fprintf(f,"%d\t",matrix[i][j]);}
		else
{vect[j]=random()%10;
fprintf(f,"%d\t",vect[j]);}
		fprintf(f,"\n");
	}
	for (i=0;i<N;i++)
		{long tmp=0;
		for (j=0;j<N;j++)
			tmp+=matrix[i][j]*vect[j];
		res[i]=tmp;
		fprintf(f,"%d\t",res[i]);
	}
	fclose(f);
}
