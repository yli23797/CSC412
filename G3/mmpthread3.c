#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 2000
#define P 2000
#define M 2000
#define NUM_THREADS 2000

int A[N][P]; // op1
int B[P][M]; // op2
int C[N][M]; // result

int num = 1;

void *calc( void *param)
{
	int i, j, k;
	long myid;
	myid = (long)  param;	

	for(i = myid - 1; i< myid; i++)
	{
		//printf("%d row number\n", i);
		for(j=0; j<M; j++)
		{
			for(k=0; k<P; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			   	//printf("C[%d][%d] = %d\n", myid, i, j, C[i][j]);	
			}
		}
	}
	pthread_exit(NULL);	
}

int main()
{
	int i, j, k;
	
	pthread_t tids[NUM_THREADS];
	long t;
	void *status;
	
	for(i = 0; i < N; i++)
	{
		for(k=0; k < P; k++)
		{
			A[i][k] = rand()%10;
		}
	}
	for(i = 0; i < M; i++)
	{
		for(k=0; k < N; k++)
		{
			B[i][k] = rand()%10;
		}
	}
	for(i = 0; i < N; i++)
	{
		for(k=0; k < M; k++)
		{
			C[i][k] = 0;
		}
	}

	/*printf("A: \n");
	for(i = 0; i < N; i++)
	{
		for(k=0; k < P; k++)
		{
			printf("%3d ", A[i][k]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("B: \n");
	for(i = 0; i < M; i++)
	{
		for(k=0; k < N; k++)
		{
			printf("%3d ", B[i][k]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	
	for (t=0; t<NUM_THREADS; t++)
		{
			pthread_create( &tids[t], NULL, calc, (void*) t+1);
		}
		
	for (t=0; t < NUM_THREADS; t++)
		{
			pthread_join( tids[t], &status );
		
		}

	/*printf("C: \n");
	for(i=0; i < N; i++)
	{
		for (k=0; k<M; k++)
		{
			printf("%3d ", C[i][k]);
		}
		printf("\n");
	}
	printf("\n");*/

}