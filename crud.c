#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

int generateNewId() {
    struct User user;
    int lastId = 0;

    FILE *file = fopen("users.txt", "r");

    if (file == NULL) {
        return 1; // First record
    }

    while (fscanf(file, "%d | %[^|] | %d", &user.id, user.name, &user.age) != EOF) {
        lastId = user.id;
    }

    fclose(file);
    return lastId + 1;
}

void createUser() {
    struct User user;
    FILE *file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    user.id = generateNewId();

    if (user.id == -1) {
        printf("Something went wrong\n");
        exit(2);
    }

    printf("\n--- Create User ---\n");

    printf("Enter User Name: ");
    getchar(); // consume newline
    scanf("%[^\n]", user.name);

    printf("Enter User Age: ");
    scanf("%d", &user.age);

    fprintf(file, "%d | %s | %d\n", user.id, user.name, user.age);
    fclose(file);

    printf("\n User Record Created Successfully!\n\n");
}

void readUsersData() {
    struct User user;
    FILE *file = fopen("users.txt", "r");

    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n--- All Users ---\n");
    while (fscanf(file, "%d | %[^|] | %d", &user.id, user.name, &user.age) != EOF) {
        printf("%d | %s | %d\n", user.id, user.name, user.age);
    }

    fclose(file);
}

// Common function to modify for update and delete user
void modifyUsersRecordsByID(FILE *file, FILE *temp, int targetId, const char *action) {
    struct User user;
    bool recordFound = false;

    while (fscanf(file, "%d | %[^|] | %d", &user.id, user.name, &user.age) != EOF) {
        if (user.id == targetId) {
            recordFound = true;

            if (strcmp(action, "update") == 0) {
                printf("New Name: ");
                getchar();
                scanf("%[^\n]", user.name);
                printf("New Age: ");
                scanf("%d", &user.age);
                fprintf(temp, "%d | %s | %d\n", user.id, user.name, user.age);
            }
            // For delete, we simply skip writing the record
        } else {
            fprintf(temp, "%d | %s | %d\n", user.id, user.name, user.age);
        }
    }

    if (!recordFound){
        printf("\n User not found.\n");

    }
    else if (strcmp(action, "update") == 0){
        printf("\n User updated successfully.\n");

    }    
    else if (strcmp(action, "delete") == 0){
        printf("\n User deleted successfully.\n");
        
    }    
}

// Common function to handle both update and delete user file opening
void modifyUser(int id, const char *action) {
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL) {
        printf("Error in opening users file!\n");
        return;
    }

    if (temp == NULL) {
        printf("Error in opening temporary file!\n");
        return;
    }

    modifyUsersRecordsByID(file, temp, id, action);

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");
}


void updateUser() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    modifyUser(id, "update");
}

void deleteUser() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    modifyUser(id, "delete");
}

void runProgram() {
    int choice;

    do {
        printf("\n\n--- User Records ---\n");
        printf("1. Create User\n");
        printf("2. View All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createUser();
                break;

            case 2:
                readUsersData();
                break;

            case 3:
                updateUser();
                break;

            case 4:
               
                deleteUser();
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 5);
}

int main() {
    runProgram();
    return 0;
}
