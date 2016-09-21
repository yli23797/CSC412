#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 6

#define gnum 7
int grid[gnum][gnum];
int grid2[gnum][gnum];


void *calc( void *param)
{
	long myid;
	int i;
	int j;
	myid = (long) param;
	int inum;
	int jnum = 7;
	int sum;
	
	if(myid == 1)
	{
		i = 1;
		inum = 2;
	}
	else if (myid == 2)
	{
		i = 2;
		inum = 3;
	}
	else if (myid == 3)
	{
		i = 3;
		inum = 4;
	}
	else if (myid == 4)
	{
		i = 4;
		inum = 5;
	}
	else if (myid == 5)
	{
		i = 5;
		inum = 6;
	}
	
	else if (myid == 6)
	{
		i = 6;
		inum = 7;
	}
		
	for (i; i < inum; i++)
	{
		for (j = 1; j < jnum; j++)
		{		
			sum = grid[i-1][j-1] + grid[i-1][j]  + grid[i-1][j+1]  + grid[i][j-1] +grid[i][j+1] +grid[i+1][j-1] + grid[i+1][j] + grid[i+1][j+1] + grid[i][j];
			
			if(sum %10 == 0)
			{
				grid2[i][j] = 0;
			}
			else if (sum < 50)
			{
				grid2[i][j] = grid[i][j] + 3;
			}
			else if (sum > 50)
			{
				if(sum > 150)
				{
					grid2[i][j] = 1;	
				}
				else 
				{
					grid2[i][j] = grid[i][j] - 3;

					if(grid2[i][j]<0)
					{
						grid2[i][j] = 0;
					}
				}
			}	
		}
	}
			
	pthread_exit(NULL);	
}

int main()
{
	pthread_t tids[NUM_THREADS];
	long t;
	void *status;
	int i, j, num, sum;	
	
	for(i=0;i<gnum+1;i++)
	{
		grid[0][i] = 0;	
		grid[i][0] = 0;	
		grid[gnum][i] = 0;
		grid[i][gnum] = 0;
	}	
	
	for (i = 1; i < gnum; i++)
	{
		for (j = 1; j < gnum; j++)
		{
			num = rand()%20;
			grid[i][j] = num;
		}
	}
	
	printf("------ Gen 1 ------\n");
	for (i = 0; i < gnum + 1; i++)
	{
		for (j = 0; j < gnum + 1; j++)
		{
			printf(" %2d", grid[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0; i < gnum +1; i++)
	{
		for (j = 0; j < gnum +1; j++)
		{
			num = 0;
			grid2[i][j] = num;	
		}
	}
	
	for (t=0; t<NUM_THREADS; t++)
		{
			pthread_create( &tids[t], NULL, calc, (void*) t+1);
		}
		
	for (t=0; t < NUM_THREADS; t++)
		{
			pthread_join( tids[t], &status );
		
		}
	
	for (i = 1; i < gnum; i++)
		{
			for (j = 1; j < gnum; j++)
			{			
				grid[i][j] = grid2[i][j];			
			}
		}	
	
	printf("\n\n------ Gen 2 ------\n");
	for (i = 0; i < gnum + 1; i++)
	{
		for (j = 0; j < gnum + 1; j++)
		{
			printf(" %2d", grid[i][j]);
		}
		printf("\n");
	}
		
	return 0;
}