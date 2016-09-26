#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define seed 0

struct matrix{
    int rows;
    int cols;
    int **array;
};
typedef struct matrix* Matrix;


Matrix new_matrix(int rows, int cols){
    Matrix m = (Matrix)malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->array = (int **)malloc(rows * sizeof(int *));
    int i;
    for (i = 0; i < rows; ++i){
        m->array[i] = (int *)malloc(cols * sizeof(int));
    }
    return m;
}

void delete_matrix(Matrix m){
    int i;
    for (i = 0; i < m->rows; ++i){
        free(m->array[i]);
    }
    free(m->array);
    free(m);
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
            printf(" %3d", m->array[i][j]);
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
    int i, j, k, result;
    #pragma omp parallel for private(i, j, k, result) shared(a, b, c)
    for (int i = 0; i < c->rows; ++i){
//        if (omp_in_parallel()) {puts("Function is parallel");}
//        printf("Using %d threads\n", omp_get_num_threads());
        for (int j = 0; j < c->cols; ++j){
            result = 0;
            for (int k = 0; k < a->cols; ++k){
                result += (a->array[i][k] * b->array[k][j]);
            }
            c->array[i][j] = result;
        }
    }
    return c;
}

int main(int argc, char* argv[]){
    srand(seed);
//    Matrix a = new_matrix(1000, 1000);
//    randomize_matrix(a, 10);
//    Matrix b = new_matrix(1000, 1000);
//    randomize_matrix(b, 10);
    int i, j, k, l, mod;
    puts("Enter the number of rows for the first matrix");
    scanf("%d", &i);
    puts("Enter the number of columns for the first matrix");
    scanf(" %d", &j);
    puts("Enter the number of rows for the second matrix");
    scanf(" %d", &k);
    puts("Enter the number of columns for the second matrix");
    scanf(" %d", &l);
    Matrix a = new_matrix(i, j);
    Matrix b = new_matrix(k, l);
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

    delete_matrix(a);
    delete_matrix(b);
    if (c != 0){
        printf("\n-----Matrix C-----\n");
        print_matrix(c);
        delete_matrix(c);
    }
}
