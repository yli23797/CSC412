#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

void CheckCmdArg( int argc )
{
    if ( argc != 3 )
    {
        printf( "ERROR: Please enter command line arguement.\n" );
        printf( "usage: ./prog matrix1 matrix2 \n" );
        exit(1);
    }
}

int **getArray( int n, int m )
{
    int i;
    int **table;
    table = malloc( n * sizeof( int* ));

    //check if allocation succeeded.
    if( table == NULL )
    {
        printf( "ERROR: Unable to allocate memory.\n" );
        exit(1);
    }

    for(i = 0 ; i < n; i++)
    {
        table[i] = malloc( m * sizeof( int ) );
        //check if allocation succeeded.
        if( table[i] == NULL )
        {
            printf( "ERROR: Unable to allocate memory.\n" );
            exit(1);
        }
    }
    return table;
}

void free2Darray( int** p, int N )
{
    int i;
    for(i = 0 ; i < N ; i++)
    {
        free(p[i]);
    }
    free(p);
}

int **readFIle( char* filename, int& n, int& m )
{
    FILE *dataFile;

    dataFile = fopen( filename, "r" );
}

int main( int argc, char *argv[] )
{
    CheckCmdArg( argc );

    int **matrix1;
    int **matrix2;
    int size1;
    int size2;

    //free2Darray( matrix1, size1 );
    //free2Darray( matrix2, size2 );
    return 0;
}