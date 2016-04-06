
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

void read_from_file(char *in,int *N,int **matrix,int *vector)
{
	FILE *input;
	int i,j;
	
	if ((input=fopen(in,"r"))==NULL)
	{
		printf("Error in the input file\n");
		exit(-1);
	}
	fscanf(input,"%d",N);
	matrix=(int**)malloc(sizeof(int*)*(*N));
	vector=(int*)malloc(sizeof(int)*(*N));
	for (i=0;i<(*N);i++)
	{
		matrix[i]=(int*)malloc(sizeof(int)*(*N));
		for (j=0;j<(*N);j++)
			fscanf(input,"%d\t",&matrix[i][j]);
	}
	for (i=0;i<(*N);i++)
		fscanf(input,"%d\t",&vector[i]);
	fclose(input);
}
void write_to_file(char *out,int N,int* res)
{
	int i;
	FILE *output;
	output=fopen(out,"w");
	for (i=0;i<N;i++)
		fprintf(output,"%d\t",res[i]);
	fclose(output);
}
void del_all(int N,int **matrix,int *vector)
{
	int i;
	for (i=0;i<N;i++)
		free(matrix[i]);
	free(matrix);
	free(vector);
}
void main(int argc, char *argv[])
{
	int shmid, status;
	int **matrix;
	int *vect;
	int N;
	int *result;
	if (argv[1]==NULL || argv[2]== NULL)
	{
		printf("In/Out files are unknown\n");
		exit(-1);
	}
	if (argv[3]==NULL)
	{	
		printf("Не задано количество процессов");
		exit(-1);
	}
	int procs=atoi(argv[3]);
	FILE *input;
	int i,j;
	
	if ((input=fopen(argv[1],"r"))==NULL)
	{
		printf("Error in the input file\n");
		exit(-1);
	}
	fscanf(input,"%d",&N);
	matrix=(int**)malloc(sizeof(int*)*(N));
	vect=(int*)malloc(sizeof(int)*(N));
	for (i=0;i<N;i++)
	{
		matrix[i]=(int*)malloc(sizeof(int)*N);
		for (j=0;j<N;j++)
			{fscanf(input,"%d",&matrix[i][j]);
			printf("%d ",matrix[i][j]);
			}
		printf("\n");
	}
	for (i=0;i<N;i++)
		{fscanf(input,"%d",&vect[i]);
		printf("%d ",vect[i]);
		}
	fclose(input);
	//read_from_file(argv[1],&N,matrix,vect);
	
	pid_t pid;
	key_t key;


	key=ftok(".",0);
	shmid = shmget(key,N*sizeof(int),IPC_CREAT|0666);
	result = (int *) shmat(shmid, NULL, 0);
	int k;
	int currow=0;
	int step;
	for (k=1;k<=procs;k++)
	{
		
		step=N/procs+(int)(N%procs==k);
		pid = fork();
		if (pid == 0) 
		{
			for (i=currow;i<currow+step;i++)
			{
				int tmp=0;
				for (j=0;j<N;j++)
					tmp+=matrix[i][j]*vect[j];
				result[i]=tmp;
				}
			exit(0);
		}
		waitpid(pid,&status,0);

		currow+=step;
		printf("\n%d %d %d",currow,step,k);
	
	}
	
	//write_to_file(argv[2],N,result);
	FILE *output;
	output=fopen(argv[2],"w");
	for (i=0;i<N;i++)
		fprintf(output,"%d ",result[i]);
	fclose(output);\
	//del_all(N,matrix,vect);
	for (i=0;i<N;i++)
		free(matrix[i]);
	free(matrix);
	free(vect);

	shmdt((void *) result); 
	shmctl(shmid, IPC_RMID, NULL);
	exit(0);
}
void Child(int rownum,int N,int *row,int *vec,int result[])
{
	int tmp=0;
	int i;
	for (i=0;i<N;i++)
		tmp=row[i]*vec[i];
	result[rownum]=tmp;
}
