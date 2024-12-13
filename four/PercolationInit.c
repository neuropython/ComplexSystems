#include <stdio.h>
#include <stdlib.h>

int main(double probability, int rows, int cols) {
    char makeMatrix[500];
    snprintf(makeMatrix, sizeof(makeMatrix), "./MatrixGen.exe %d %d %d", probability, cols, rows);
    printf("Command: %s\n", makeMatrix);
    int result = system(makeMatrix);

    return 0;
    
}