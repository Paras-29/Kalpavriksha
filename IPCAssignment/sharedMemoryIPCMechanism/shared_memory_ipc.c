#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

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

void sort(int *arr, int n){
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

int createSharedMemory(int size){
    key_t key = ftok("shmfile", 75);
    return shmget(key, size, 0666 | IPC_CREAT);
}

int* attachSharedMemory(int shmid){
    return (int *)shmat(shmid, NULL, 0);
}

int main(){
    int n;
    int *inputArr = takeInput(&n);

    int shmid = createSharedMemory((n + 1) * sizeof(int));
    int *shmArr = attachSharedMemory(shmid);

    shmArr[0] = n;
    for (int i = 0; i < n; i++) {
        shmArr[i + 1] = inputArr[i];
    }

    printf("\nBefore Sorting:\n");
    display(inputArr, n);

    if (fork() == 0) {
        int size = shmArr[0];
        sort(&shmArr[1], size);
    }
    else {
        wait(NULL);

        printf("\nAfter Sorting:\n");
        display(&shmArr[1], shmArr[0]);

        shmdt(shmArr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    free(inputArr);
    return 0;
}
