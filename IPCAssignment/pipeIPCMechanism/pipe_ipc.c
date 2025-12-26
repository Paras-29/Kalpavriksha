#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int* takeInput(int *n) {
    printf("Enter number of elements: ");
    scanf("%d", n);

    int *arr = (int *)malloc((*n) * sizeof(int));

    printf("Enter elements:\n");
    for (int i = 0; i < *n; i++) {
        scanf("%d", &arr[i]);
    }

    return arr;
}

void display(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sort(int *arr, int n) {
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

void writeToPipe(int fd, int *arr, int n) {
    write(fd, &n, sizeof(int));
    write(fd, arr, n * sizeof(int));
}

int* readFromPipe(int fd, int *n) {
    read(fd, n, sizeof(int));

    int *arr = (int *)malloc((*n) * sizeof(int));
    read(fd, arr, (*n) * sizeof(int));

    return arr;
}

int main() {
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0){
        close(pipe1[1]);
        close(pipe2[0]);

        int n;
        int *arr = readFromPipe(pipe1[0], &n);

        sort(arr, n);
        writeToPipe(pipe2[1], arr, n);
        free(arr);
    }
    else{
        close(pipe1[0]);
        close(pipe2[1]);

        int n;
        int *arr = takeInput(&n);

        printf("\nBefore Sorting:\n");
        display(arr, n);

        writeToPipe(pipe1[1], arr, n);

        int size;
        int *sortedArr = readFromPipe(pipe2[0], &size);

        printf("\nAfter Sorting:\n");
        display(sortedArr, size);

        free(arr);
        free(sortedArr);
    }

    return 0;
}
