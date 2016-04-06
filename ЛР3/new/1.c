#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

void main(int argc,char *argv[])
{
	FILE *f;
	f=fopen("100by100.txt","w");
	int i,j;
	int matrix[150][150];
	int vect[150];
	int res[150];
	for (i=0;i<151;i++)
	{
		for (j=0;j<150;j++) 
		if(i<151)
			{matrix[i][j]=random()%10;
fprintf(f,"%d\t",matrix[i][j]);}
		else
{vect[j]=random()%10;
fprintf(f,"%d\t",vect[j]);}
		fprintf(f,"\n");
	}
	for (i=0;i<150;i++)
		{long tmp=0;
		for (j=0;j<150;j++)
			tmp+=matrix[i][j]*vect[j];
		res[i]=tmp;
		fprintf(f,"%ld\t",res[i]);
	}
	fclose(f);
}
