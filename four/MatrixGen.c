#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Initializes a matrix with random data based on the given probability.
 *
 * @param p The probability of a cell being 1.
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @return A pointer to the initialized matrix.
 * Compile command:
 * gcc MatrixGen.c -o MatrixGen
 */

int** matrixF(int rows, int cols) {
    // Allocate memory for Rows
    int** matrix = (int**)malloc(rows * sizeof(int*));

    // Allocate memory for Columns 
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

int randomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int** initializeMatrixWithData(double probabilty, int rows, int cols) {
    int** matrix = matrixF(rows,cols);

    srand(time(NULL));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = randomNumber(0, 1) < probabilty ? 1 : 0;
        } 
    }

    // Print the matrix
    printf("---NUMERICAL REPRESENTATION---\n");
    printMatrix(matrix, rows, cols);
    printf("---GRAPHICAL REPRESENTATION---\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
           if (matrix[i][j] == 0) {
            printf("\033[31m# \033[0m"); // RED
           } else {
            printf("\033[32m# \033[0m"); // GREEN
           };
        }
        printf("\n");
    }
}

void burningMethod (int** matrix, int rows, int cols) {
    // Loop threw the first row
    for (int i = 0; i < cols; i++) {
        if (matrix[0][i] == 1) {
            matrix[0][i] = 2;
        }
    }

    // Start the burning method

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 2) {
                if

}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <probability> <rows> <cols>\n", argv[0]);
        return 1;
    }

    double probability = atof(argv[1]);
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    printf("%d\n", rows);
    printf("%d\n", cols);

    int** matrix = initializeMatrixWithData(probability, rows, cols);
    BurningMethod(matrix, rows, cols);
    printMatrix(matrix, rows, cols);
    // Free the allocated memory for the matrix
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}