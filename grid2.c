#include <stdio.h>

int main()
{
	int gnum = 7;
	int grid[gnum][gnum];
	int grid2[gnum][gnum];
	
	int i, j, k, num, sum;
	int num1, num2;
	
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
		//	printf("%d", grid[i][j]);
		}
	//	printf("\n");
	}
	printf("A---\n");
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
		//printf("%2d", grid2[i][j]);
		}
	//	printf("\n");
	}
	
	printf("GRID 2\n");
	for (i = 0; i < gnum + 1; i++)
	{
		for (j = 0; j < gnum + 1; j++)
		{
			printf(" %2d", grid2[i][j]);
		}
		printf("\n");
	}

for(k=0; k<5; k++)
{
		
	for (i = 1; i < gnum; i++)
	{
		for (j = 1; j < gnum; j++)
		{		
			sum = grid[i-1][j-1] + grid[i-1][j]  + grid[i-1][j+1]  + grid[i][j-1] +grid[i][j+1] +grid[i+1][j-1] + grid[i+1][j] + grid[i+1][j+1] + grid[i][j];
			//printf("sum is %d\n", sum);
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
for (i = 1; i < gnum; i++)
		{
			for (j = 1; j < gnum; j++)
			{
			
				grid[i][j] = grid2[i][j];
			//printf("%2d", grid2[i][j]);
			}
		//	printf("\n");
		}	
	
	printf("\n\nB---\n");
	for (i = 0; i < gnum + 1; i++)
	{
		for (j = 0; j < gnum + 1; j++)
		{
			printf(" %2d", grid[i][j]);
		}
		printf("\n");
	}
	
}
	
	return 0;
}