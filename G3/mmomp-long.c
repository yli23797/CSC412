/**
 *
 * Build with -fopenmp.
 *
 * Run with -i or --interactive for interactive mode.
 *
 * Keep in mind that all matrices are using 32-bit signed integers.
 *
 *
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define seed 0

struct matrix{
    int rows;
    int cols;
    long long int **array;
};
typedef struct matrix* Matrix;

enum return_val{ NO_MEM=1, BAD_ARGS, FILE_ERROR};

void out_of_mem(Matrix);

Matrix new_matrix(int rows, int cols){
    Matrix m = (Matrix)malloc(sizeof(struct matrix));
    if (m == NULL){
        out_of_mem(m);
    }
    m->rows = rows;
    m->cols = cols;
    m->array = (long long int **)malloc(rows * sizeof(long long int *));
    if (m->array == NULL){
        out_of_mem(m);
    }
    int i, j;
    for (i = 0; i < rows; ++i){
        m->array[i] = (long long int *)malloc(cols * sizeof(long long int));
        if (m->array[i] == NULL){
            out_of_mem(m);
        }
    }
    return m;
}

void delete_matrix(Matrix m){
    //NB: this can be used on a matrix that didn't allocate properly, and even on a NULL pointer
    int i;
    if (m != NULL){
        if (m->array != NULL){
            for (i = 0; i < m->rows; ++i){
                if( m->array[i] != NULL){
                    free(m->array[i]);
                }
            }
            free(m->array);
        }
        free(m);
    }
}

void out_of_mem(Matrix m){
    //Currently, this can cause a memory leak since it only frees the current matrix.
    //I'm considering adding a structure that holds pointers to all allocated objects, so all can be freed.
    printf( "ERROR: Unable to allocate memory.\n" );
    delete_matrix(m);
    exit(NO_MEM);
}

void randomize_matrix(Matrix m, int modulo){
    int i, j;
    for (i = 0; i < m->rows; ++i){
        for (j = 0; j < m->cols; ++j){
            m->array[i][j] = rand() % modulo;
        }
    }
}

void print_matrix(Matrix m){
    int i, j;
    for (i = 0; i < m->rows; ++i){
        for (j = 0; j < m->cols; ++j){
            printf(" %3ld", m->array[i][j]);
        }
        puts("");
    }
}

Matrix multiply(Matrix a, Matrix b){
    if (a->cols != b->rows){
        puts("Error! Matrix A must have as exactly as many columns as Matrix B has rows!");
        return 0;
    }
    Matrix c = new_matrix(a->rows, b->cols);
    int i, j, k;
    long long int result;
    #pragma omp parallel for private(i, j, k, result) shared(a, b, c)
    for (i = 0; i < c->rows; ++i){
//        if (omp_in_parallel()) {puts("Function is parallel");}
//        printf("Using %d threads\n", omp_get_num_threads());
        for (j = 0; j < c->cols; ++j){
            result = 0;
            for (k = 0; k < a->cols; ++k){
                result += (a->array[i][k] * b->array[k][j]);
            }
            c->array[i][j] = result;
        }
    }
    return c;
}

Matrix readfile( char* filename, int* rowNum, int* columnNum )
{
    FILE* dataFile;

    dataFile = fopen( filename, "r" );
    
    if( dataFile == NULL )
    {
        printf("ERROR: Can't open %s\n", filename );
        exit(FILE_ERROR);
    }

    fscanf( dataFile, "%d", rowNum );
    fscanf( dataFile, "%d", columnNum );

    Matrix m = new_matrix( *rowNum, *columnNum );
    int i, j;
    // This code assume the input format is correct.
    for(i = 0; i < *rowNum; i++ )
    {
        for(j = 0; j < *columnNum; j++ )
        {
            fscanf( dataFile, "%lld", &(m->array[i][j]) );
        }
    }
    fclose( dataFile );
    return m;
}

void outFileName( char *temp, int row, int column )
{
    char filename[1024] = "testResult/result";
    char rowstr[50] = "";
    char colstr[50] = "";
    sprintf( rowstr, "%d", row );
    sprintf( colstr, "%d", column );

    strcat( filename, rowstr );
    strcat( filename, "_" );
    strcat( filename, colstr );
    strcat( filename, "_openmp" );

    strcpy( temp, filename );
}

void outPutMatrix( Matrix m )
{
    FILE* outFile;
    char filename[1024] = {}; //initializing with zeroes guarantees the string will be null-terminated

    outFileName( filename, m->rows, m->cols );

    outFile = fopen( filename, "w+" );
    if( outFile == NULL )
    {
        printf("ERROR: Can't open %s\n", filename );
        exit(1);
    }

    fprintf( outFile, "%d ", m->rows );
    fprintf( outFile, "%d\n", m->cols );
    int i, j;
    for( i = 0; i < m->rows; i++)
    {
        for( j = 0; j < m->cols; j++ )
        {
            fprintf( outFile, "%lld ", m->array[i][j] );
        }
        fprintf( outFile, "\n" );
    }

    fclose( outFile );
}


void interactive_mode(){
    int rowNum1, columnNum1, rowNum2, columnNum2, mod;
    Matrix a, b;
    
    puts("Running in interactive mode.");
    puts("Enter the number of rows for the first matrix");
    scanf("%d", &rowNum1);
    puts("Enter the number of columns for the first matrix");
    scanf(" %d", &columnNum1);
    puts("Enter the number of rows for the second matrix");
    scanf(" %d", &rowNum2);
    puts("Enter the number of columns for the second matrix");
    scanf(" %d", &columnNum2);
    a = new_matrix(rowNum1, columnNum1);
    b = new_matrix(rowNum2, columnNum2);
    puts("The matrices will be populated with random integers.\n Choose the modulo for the first matrix.");
    scanf(" %d", &mod);
    randomize_matrix(a, mod);
    puts("Choose the modulo for the second matrix.");
    scanf(" %d", &mod);
    randomize_matrix(b, mod);


    printf("\n-----Matrix A-----\n");
    print_matrix(a);
    printf("\n-----Matrix B-----\n");
    print_matrix(b);

    omp_set_num_threads(omp_get_max_threads());
    Matrix c = multiply(a, b);

    //You only get here if there were no issues allocating memory.  This is why there can be leaks.
    //I intend to fix this.
    delete_matrix(a);
    delete_matrix(b);
    if (c != 0){
        printf("\n-----Matrix C-----\n");
        print_matrix(c);
        delete_matrix(c);
    }
}

void batch_mode(char* argv[]){
    
    int rowNum1;
    int columnNum1;
    int rowNum2;
    int columnNum2;
    
    Matrix matrix1 = readfile( argv[1], &rowNum1, &columnNum1 );
    Matrix matrix2 = readfile( argv[2], &rowNum2, &columnNum2 );

    // matrix multiplication code here.
    Matrix matrix3 = multiply(matrix1, matrix2);

    outPutMatrix( matrix3 );

    // free memory
    delete_matrix(matrix1);
    delete_matrix(matrix2);
    delete_matrix(matrix3);
}

void CheckCmdArg( int argc, char* argv[] ){
    int i;
    if (argc != 3){
        for (i = 0; i < argc; ++i){
            if (strcmp("-i", argv[i]) == 0 || strcmp("--interactive", argv[i]) == 0){
                return interactive_mode();
            }
        }
        printf( "usage: %s matrix1 matrix2 \n", argv[0] );
        exit(BAD_ARGS);
    }
    else{
        batch_mode(argv);
    }
}

int main(int argc, char* argv[]){
    srand(seed);
    CheckCmdArg(argc, argv);

}
