#include<stdio.h>

/** 
 * @param matrix 
 * Compile command: 
 * gcc BurningMethod.c -o MatrixGen
 * **/

int main (int** matrix) {
    int rows = sizeof(matrix);
    int cols = sizeof(matrix[0]);
    printf("Rows: %d\n", rows);
    printf("Columns: %d\n", cols);
}
