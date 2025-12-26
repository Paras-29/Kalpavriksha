#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int* takeArrayInput(int *n) {
    printf("Enter number of elements: ");
    scanf("%d", n);

    int *arr = (int *)malloc((*n) * sizeof(int));

    printf("Enter elements:\n");
    for (int i = 0; i < *n; i++) {
        scanf("%d", &arr[i]);
    }

    return arr;
}

void displayArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void writeArrayToFile(int *arr, int n) {
    FILE *fp = fopen("data.txt", "w");

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", arr[i]);
    }

    fclose(fp);
}

int* readArrayFromFile(int *n) {
    FILE *fp = fopen("data.txt", "r");

    fscanf(fp, "%d", n);
    int *arr = (int *)malloc((*n) * sizeof(int));

    for (int i = 0; i < *n; i++) {
        fscanf(fp, "%d", &arr[i]);
    }

    fclose(fp);
    return arr;
}


void sortArray(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            
            if (arr[j] > arr[j + 1]){
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    int *arr = takeArrayInput(&n);

    printf("\nBefore Sorting:\n");
    displayArray(arr, n);

    writeArrayToFile(arr, n);

    if (fork() == 0){
        int size;
        int *childArr = readArrayFromFile(&size);

        sortArray(childArr, size);
        writeArrayToFile(childArr, size);

        free(childArr);
    }
    else {
        sleep(1);

        int size;
        int *sortedArr = readArrayFromFile(&size);

        printf("\nAfter Sorting:\n");
        displayArray(sortedArr, size);

        free(sortedArr);
    }

    free(arr);
    return 0;
}
