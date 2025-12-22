#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define DB_FILE "../resource/accountDB.txt"

pthread_mutex_t fileLock;

float readBalance() {
    FILE *file = fopen(DB_FILE, "r");
    float balance = 0;

    if (file != NULL) {
        fscanf(file, "%f", &balance);
        fclose(file);
    }

    return balance;
}

void writeBalance(float balance) {
    FILE *file = fopen(DB_FILE, "w");
    fprintf(file, "%.2f", balance);
    fclose(file);
}

void processTransaction(int choice, float amount, char *response){
    pthread_mutex_lock(&fileLock);

    float balance = readBalance();

    if (choice == 1) {
        if (amount > balance) {
            sprintf(response, "Withdrawal failed. Insufficient balance.");
        }
        else{
            balance -= amount;
            writeBalance(balance);
            sprintf(response, "Withdraw successful. Balance: %.2f", balance);
        }
    }
    else if (choice == 2) {
        balance += amount;
        writeBalance(balance);
        sprintf(response, "Deposit successful. Balance: %.2f", balance);
    }
    else if (choice == 3){
        sprintf(response, "Current Balance: %.2f", balance);
    }

    pthread_mutex_unlock(&fileLock);
}

void *handleClient(void *arg) {
    int clientSocket = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytes <= 0) {
            break;
        }

        int choice;
        float amount;

        sscanf(buffer, "%d %f", &choice, &amount);

        if (choice == 4) {
            break;
        }

        memset(response, 0, BUFFER_SIZE);
        processTransaction(choice, amount, response);

        send(clientSocket, response, strlen(response), 0);
    }

    close(clientSocket);
    return NULL;
}

int createServerSocket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void bindServer(int serverSocket) {
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
}

void startListening(int serverSocket) {
    listen(serverSocket, 5);
}

int main() {
    pthread_mutex_init(&fileLock, NULL);

    int serverSocket = createServerSocket();
    bindServer(serverSocket);
    startListening(serverSocket);

    printf("ATM Server running on port %d...\n", PORT);

    while (1){
        int *clientSocket = malloc(sizeof(int));
        *clientSocket = accept(serverSocket, NULL, NULL);

        pthread_t thread;
        pthread_create(&thread, NULL, handleClient, clientSocket);
        pthread_detach(thread);
    }

    close(serverSocket);
    pthread_mutex_destroy(&fileLock);
    return 0;
}
