/**

Diana Lewis, Yanlin Li, Marty Moradian, Stephen Chilcote, Alexander Houser

 **/

#include <stdio.h>

/*
void zero_perimeter(int size, int grid[][size]){
    
    int i;
    
    for(i=0; i < size + 1; i++)
    {
        grid[0][i] = 0; 
        grid[i][0] = 0; 
        grid[size][i] = 0;
        grid[i][size] = 0;
    }
}
*/

void zero_grid(int size, int grid[][size]){
	int i, j;
	for (i = 0; i < size + 1; i++) {
        for (j = 0; j < size +1; j++) {
            grid[i][j] = 0;
        }
    }
}


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

int main() {
    //size = width/height of the square grid
    int size = 7;
    int grid[size][size];
    int grid2[size][size];
    
    int i, j, k, num, sum;
    int num1, num2;
    
    //fill grids with zeroes
    zero_grid(size, grid);
    zero_grid(size, grid2);
    
    //initialize main grid
          
    for (i = 1; i < size; i++)
    {
        for (j = 1; j < size; j++)
        {
            num = rand()%20;
            grid[i][j] = num;
        //  printf("%d", grid[i][j]);
        }
    //  printf("\n");
    }
    
    printf("A---\n");
    print_grid(size, grid);      
    
    printf("GRID 2\n");
    print_grid(size, grid2);
    

	for(k=0; k<5; k++)
	{
		for (i = 1; i < size; i++)
		{
			for (j = 1; j < size; j++)
			{       
				sum = sum_neighbors(i, j, size, grid);
				grid2[i][j] = result(i, j, size, grid, sum);
			}
		}
		for (i = 1; i < size; i++)
		{
			for (j = 1; j < size; j++)
			{
					
				grid[i][j] = grid2[i][j];
				//printf("%2d", grid2[i][j]);
			}
				//  printf("\n");
		}   
			
		printf("\n\nB---\n");
		print_grid(size, grid);
	}
    return 0;
}
