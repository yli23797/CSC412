#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#define THREAD_NUM 100;

void CheckCmdArg( int argc )
{
    if ( argc != 2 )
    {
        printf( "ERROR: Please enter command line arguement.\n" );
        printf( "usage: ./prog matrix1 matrix2 \n" );
        exit(1);
    }
}

int** getArray( int n, int m )
{
    int i;
    int** table;
    table = malloc( n * sizeof(int*) );

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

int** readFIle( char* filename, int* rowNum, int* columnNum )
{
    FILE* dataFile;

    int** matrix;

    dataFile = fopen( filename, "r" );
    if( dataFile == NULL )
    {
        printf("ERROR: Can't open %s\n", filename );
        exit(1);
    }

    fscanf( dataFile, "%d", *&rowNum );
    fscanf( dataFile, "%d", *&columnNum );

    matrix = getArray( *rowNum, *columnNum );

    // This code assume the input format is correct.
    for( int i = 0; i < *rowNum; i++ )
    {
        for( int j = 0; j < *columnNum; j++ )
        {
            fscanf( dataFile, "%d", &matrix[i][j] );
        }
    }
    fclose( dataFile );
    return matrix;
}

void outFileName( char *temp, int row, int column )
{
    char filename[80] = "testResult/result";
    char rowstr[50] = "";
    char colstr[50] = "";
    sprintf( rowstr, "%d", row );
    sprintf( colstr, "%d", column );

    strcat( filename, rowstr );
    strcat( filename, "_" );
    strcat( filename, colstr );
    strcat( filename, "_pthread" );
    strcat( filename, "\0" );

    strcpy( temp, filename );
}

void outPutMatrix( int **matrix, int rowNum, int columnNum )
{
    FILE* outFile;
    char filename[80] = "";

    outFileName( filename, rowNum, columnNum );


    outFile = fopen( filename, "w+" );
    if( outFile == NULL )
    {
        printf("ERROR: Can't open %s\n", filename );
        exit(1);
    }

    fprintf( outFile, "%d ", rowNum );
    fprintf( outFile, "%d\n", columnNum );

    for( int i = 0; i < rowNum; i++)
    {
        for( int j = 0; j < columnNum; j++ )
        {
            fprintf( outFile, "%d ", matrix[i][j] );
        }
        fprintf( outFile, "\n" );
    }

    fclose( outFile );
}

int cellResult( int** matrix1, int** matrix2, int row, int column )
{
    int reslut;
    for( int i = 0; i < row; i++ )
    {
        for( int j = 0; j < column; j++ )
        {
            reslut += matrix1*matrix2;
        }
    }
    return result;
}

int main( int argc, char *argv[] )
{
    CheckCmdArg( argc );

    int **matrix1;
    int **matrix2;
    int rowNum1;
    int columnNum1;
    int rowNum2;
    int columnNum2;

    matrix1 = readFIle( argv[1], &rowNum1, &columnNum1 );
    matrix2 = readFIle( argv[2], &rowNum2, &columnNum2 );


    // matrix multiplication code here.

    outPutMatrix( matrix1, rowNum1, columnNum1 );

    // free memory
    free2Darray( matrix1, rowNum1 );
    free2Darray( matrix2, rowNum2 );

    return 0;
}