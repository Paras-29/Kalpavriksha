#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void showMenu() {
    printf("\n---- ATM MENU ----\n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Display Balance\n");
    printf("4. Exit\n");
}

int getChoice() {
    int choice;
    printf("Enter choice: ");
    scanf("%d", &choice);
    return choice;
}

float getAmount() {
    float amount;
    printf("Enter amount: ");
    scanf("%f", &amount);
    return amount;
}

int createSocket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void connectServer(int sock) {
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
}

void sendRequest(int sock, int choice, float amount){
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%d %.2f", choice, amount);
    send(sock, buffer, strlen(buffer), 0);
}

void receiveResponse(int sock) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s\n", buffer);
}

int main(){
    int sock = createSocket();
    connectServer(sock);

    while (1) {
        showMenu();
        int choice = getChoice();
        float amount = 0;

        if (choice == 1 || choice == 2) {
            amount = getAmount();
        }

        sendRequest(sock, choice, amount);

        if (choice == 4) {
            break;
        }

        receiveResponse(sock);
    }

    close(sock);
    return 0;
}
