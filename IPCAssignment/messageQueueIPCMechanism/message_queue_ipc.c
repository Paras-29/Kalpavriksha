#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long type;
    int n;
    int arr[100];
};

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

void display(int *arr, int n){
    for (int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sendMessage(int msgid, long type, int *arr, int n) {
    struct message msg;
    msg.type = type;
    msg.n = n;

    for (int i = 0; i < n; i++) {
        msg.arr[i] = arr[i];
    }

    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
}

int* receiveMessage(int msgid, long type, int *n) {
    struct message msg;

    msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), type, 0);

    *n = msg.n;
    int *arr = (int *)malloc((*n) * sizeof(int));

    for (int i = 0; i < *n; i++) {
        arr[i] = msg.arr[i];
    }

    return arr;
}

int main(){
    key_t key = ftok("msgfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    if (fork() == 0) {
        int n;
        int *arr = receiveMessage(msgid, 1, &n);

        sort(arr, n);
        sendMessage(msgid, 2, arr, n);
        free(arr);
    }
    else{
        int n;
        int *arr = takeInput(&n);

        printf("\nBefore Sorting:\n");
        display(arr, n);

        sendMessage(msgid, 1, arr, n);

        int size;
        int *sortedArr = receiveMessage(msgid, 2, &size);

        printf("\nAfter Sorting:\n");
        display(sortedArr, size);

        msgctl(msgid, IPC_RMID, NULL);

        free(arr);
        free(sortedArr);
    }

    return 0;
}
