#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PIXEL_VALUE 256

void printMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", *(matrix + i * n + j));
        }
        printf("\n");
    }
    printf("\n");
}

void swapValues(int *firstValue, int *secondValue) {
    int tempValue = *firstValue;
    *firstValue = *secondValue;
    *secondValue = tempValue;
}

int isValidCell(int matrixSize, int row, int col) {
    return (row >= 0 && row < matrixSize && col >= 0 && col < matrixSize);
}


int getAllNeighborAverage(int *sonarImage, int matrixSize, int row, int col) {
    int sum = 0, count = 0;

    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            int newRow = row + deltaRow;
            int newCol = col + deltaCol;

            if (isValidCell(matrixSize, newRow, newCol)) {
                sum += *(sonarImage + newRow * matrixSize + newCol) % MAX_PIXEL_VALUE;
                count++;
            }
        }
    }
    return sum / count;
}

void encodeSmoothedValues(int* sonarImage, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            int avg = getAllNeighborAverage(sonarImage, matrixSize, row, col);
    
            *(sonarImage + row * matrixSize + col) += avg * MAX_PIXEL_VALUE;
        }
    }
}

void decodeSmoothedValues(int* sonarImage, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
           
            *(sonarImage + row * matrixSize + col) /= MAX_PIXEL_VALUE;
        }
    }
}

void applySmoothingFilter(int* sonarImage, int matrixSize) {
    encodeSmoothedValues(sonarImage, matrixSize);
    decodeSmoothedValues(sonarImage, matrixSize);
}


void transposeMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int *a = matrix + i * n + j;
            int *b = matrix + j * n + i;

            swapValues(a, b);
        }
    }
}

void reverseRows(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        int *rowStart = matrix + i * n;
        int *rowEnd = rowStart + n - 1;
        while (rowStart < rowEnd) {

            swapValues(rowStart, rowEnd);

            rowStart++;
            rowEnd--;
        }
    }
}

void rotateMatrix90Clockwise(int *matrix, int n) {
    transposeMatrix(matrix, n);
    reverseRows(matrix, n);
}


void inputSonarMatrix(int *matrix, int n) {
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(matrix + i * n + j) = rand() % MAX_PIXEL_VALUE; 
        }
    }
}

int isValidMatrixSize(int size) {
    return (size >= 2 && size <= 10);
}


int getMatrixSize() {
    
    int size;
    
    while(1){
        printf("Enter matrix size (2-10): ");
        scanf("%d", &size);
        
        if (!isValidMatrixSize(size)) {
            printf("Invalid matrix size. Please try between 2 and 10\n");
        }
        else {
            return size;
        }

            
    }
    
}


void sonarImageMatrixSimulation() {
    
    int matrixSize = getMatrixSize();
    
    int *sonarMatrix = (int*)malloc(matrixSize * matrixSize * sizeof(int));
    
    if(sonarMatrix == NULL){
        printf("Memory is not allocated.\n");
        return;
    }

    inputSonarMatrix(sonarMatrix, matrixSize);
    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(sonarMatrix, matrixSize);
    
    rotateMatrix90Clockwise(sonarMatrix, matrixSize);
    printf("Matrix after 90 degree Clockwise Rotation:\n");
    printMatrix(sonarMatrix, matrixSize);
    
    applySmoothingFilter(sonarMatrix, matrixSize);
    printf("Matrix after Applying 3*3 Smoothing Filter:\n");
    printMatrix(sonarMatrix, matrixSize);
    
    free(sonarMatrix);
}


int main() {
    srand(time(0)); 

    sonarImageMatrixSimulation();
    
    return 0;
}