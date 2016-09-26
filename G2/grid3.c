#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 6

#define gnum 7
int grid[gnum][gnum];
int grid2[gnum][gnum];


void print_grid(int size, int grid[][size]){
    int i, j;
    for (i = 0; i < size + 1; i++)
    {
        for (j = 0; j < size + 1; j++)
        {
            printf(" %2d", grid[i][j]);
        }
        printf("\n");
    }
}

void zero_grid(int size, int grid[][size]){
	int i, j;
	for (i = 0; i < size + 1; i++) {
        for (j = 0; j < size +1; j++) {
            grid[i][j] = 0;
        }
    }
}

int sum_neighbors(int i, int j, int size, int grid[][size]) {
	return grid[i-1][j-1] + grid[i-1][j]  + grid[i-1][j+1]  + grid[i][j-1] +grid[i][j+1] +grid[i+1][j-1] + grid[i+1][j] + grid[i+1][j+1] + grid[i][j];
}

int result(int i, int j, int size, int grid[][size], int sum){
	int tmp;
	if(sum %10 == 0)
	{
		return 0;
	}
	if (sum < 50)
	{
		return grid[i][j] + 3;
	}
	if (sum > 50)
	{
		if(sum > 150)
		{
			return 1;    
		}
		else 
		{
			tmp = grid[i][j] - 3;
			if (tmp < 0)
			{
				return 0;
			}
			return tmp;
		}				
	}   
}


void *calc( void *param)
{
	long myid;
	int i;
	int j;
	myid = (long) param;
	int inum;
	int jnum = 7;
	int sum;
	
	
	int inums[NUM_THREADS];
	for (i = 1; i <= NUM_THREADS; ++i){
		inums[i] = i + 1;
	}
	
	i = myid;
	inum = inums[myid];
		
	for (i; i < inum; i++)
	{
		for (j = 1; j < jnum; j++)
		{		
			sum = sum_neighbors(i, j, gnum, grid);
			grid2[i][j] = result(i, j, gnum, grid, sum);	
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
	
	zero_grid(gnum, grid2);	
	
	//initialize main grid
	for (i = 1; i < gnum; i++)
	{
		for (j = 1; j < gnum; j++)
		{
			num = rand()%20;
			grid[i][j] = num;
		}
	}
	
	printf("------ Gen 1 ------\n");
	print_grid(gnum, grid);
	
	//initialize work grid
	zero_grid(gnum, grid2);
	
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
	print_grid(gnum, grid);
		
	return 0;
}
