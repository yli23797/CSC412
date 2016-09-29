#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#define THREAD_NUM 2

typedef struct
{
    int rows1;
    int cols1;
    int rows2;
    int cols2;
    int** matrix1;
    int** matrix2;
    int** result;
} matrixMul;

void CheckCmdArg( int argc );
int** getArray( int n, int m );
void free2Darray( int** p, int N );
matrixMul readFIle( char* filename1, char* filename2 );
void outPutMatrix( int** matrix, int rowNum, int columnNum );
void outFileName( char* temp, int row, int column );
void *calc( void *param);

matrixMul matrices;

int main( int argc, char *argv[] )
{
    int i, j, k;
    CheckCmdArg( argc );
    matrices = readFIle( argv[1], argv[2] );
    void* status;
    //outPutMatrix( matrices.matrix1, matrices.rows1, matrices.cols2);

    pthread_t tids[THREAD_NUM];
    pthread_attr_t attr;
    pthread_attr_init( &attr );

    int join_Thread = THREAD_NUM;

    if( THREAD_NUM > matrices.rows1*matrices.cols2 )
    {
        join_Thread =  matrices.rows1*matrices.cols2;
    }
    for( i = 0; i < join_Thread; i++)
    {
        pthread_create( &tids[i], NULL, calc, (void*)i+1);
    }

    for( i = 0; i < join_Thread; i++)
    {
        // how to check if thread is able to be join? 
        // right now it crashed the program if the matrix is
        // too small and THREAD_NUM is too big.
        pthread_join( tids[i], &status );
    }

    outPutMatrix( matrices.result, matrices.rows1, matrices.cols2);

    free2Darray( matrices.matrix1, matrices.rows1 );
    free2Darray( matrices.matrix2, matrices.rows2 );
    free2Darray( matrices.result, matrices.rows1 );
    return 0;
}

void CheckCmdArg( int argc )
{
    if ( argc != 3 )
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
    for( i = 0 ; i < N ; i++ )
    {
        free(p[i]);
    }
    free(p);
}

matrixMul readFIle( char* filename1, char* filename2 )
{
    matrixMul matrix;

    FILE* dataFile;
    FILE* dataFile2;

    dataFile = fopen( filename1, "r" );
    if( dataFile == NULL )
    {
        printf("ERROR: Can't open %s\n", filename1 );
        exit(1);
    }

    dataFile2 = fopen( filename2, "r" );
    if( dataFile2 == NULL )
    {
        printf("ERROR: Can't open %s\n", filename2 );
        exit(1);
    }


    fscanf( dataFile, "%d", &(matrix.rows1) );
    fscanf( dataFile, "%d", &(matrix.cols1) );

    fscanf( dataFile2, "%d", &(matrix.rows2) );
    fscanf( dataFile2, "%d", &(matrix.cols2) );

    matrix.matrix1 = getArray( matrix.rows1, matrix.cols1 );
    matrix.matrix2 = getArray( matrix.rows2, matrix.cols2 );
    matrix.result = getArray( matrix.rows1, matrix.cols2 );

    // This code assume the input format is correct.
    int i, j;
    for( i = 0; i < matrix.rows1; i++ )
    {
        for( j = 0; j < matrix.cols1; j++ )
        {
            fscanf( dataFile, "%d", &(matrix.matrix1[i][j]) );
        }
    }
    for( i = 0; i < matrix.rows2; i++ )
    {
        for( j = 0; j < matrix.cols2; j++ )
        {
            fscanf( dataFile2, "%d", &(matrix.matrix2[i][j]) );
        }
    }

    fclose( dataFile );
    fclose( dataFile2 );
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

    int i, j;
    for( i = 0; i < rowNum; i++)
    {
        for( j = 0; j < columnNum; j++ )
        {
            fprintf( outFile, "%d ", matrix[i][j] );
        }
        fprintf( outFile, "\n" );
    }

    fclose( outFile );
}

void *calc( void *param)
{
    int i, j, k;
    int t = (int)param;


    for(i = t - 1; i < t; i++)
    {
        //printf("%d row number\n", i);
        for(j = 0; j < matrices.rows1; j++)
        {
            for(k = 0; k < matrices.cols2; k++)
            {
                matrices.result[i][j] += matrices.matrix1[i][k] * matrices.matrix2[k][j];
                //printf("C[%d][%d] = %d\n", myid, i, j, C[i][j]);  
            }
        }
    }
    pthread_exit(NULL); 
}
