#include <stdio.h>
#include <stdlib.h>

struct User{
    int id;
    char name[50];
    int age;
};

int idExists(int id){
    struct User u;

    FILE *file = fopen("userRecords.txt", "r");
    if (!file){
        return 0;
    }

    while (fscanf(file, "%d | %[^|] | %d", &u.id, u.name, &u.age) != EOF){
        if (u.id == id){
            fclose(file);
            return 1; // ID exists
        }
    }

    fclose(file);
    return 0; // ID does not exist
}

void createUser(){
    struct User u;

    printf("\nCreating User Record:\n");
    printf("Enter User ID: ");
    scanf("%d", &u.id);

    printf("Enter User Name: ");
    getchar();  // consume newline
    scanf("%[^\n]", u.name);

    printf("Enter User Age: ");
    scanf("%d", &u.age);

    if(idExists(u.id)){
        printf("Error: User ID already exists!\n");
        return;
    }

    FILE *file = fopen("userRecords.txt", "a");
    if (!file){
        printf("File not open!\n");
        return;
    }

    fprintf(file, "%d | %s | %d\n", u.id, u.name, u.age);
    fclose(file);
    printf("\nUser Record Created Successfully!\n\n");
}

void readUsers(){
    struct User u;

    FILE *file = fopen("userRecords.txt", "r");
    if (!file){
        printf("No records found.\n");
        return;
    }

    printf("\n--- All Users ---\n");
    while (fscanf(file, "%d | %[^|] | %d", &u.id, u.name, &u.age) != EOF){
        printf("%d | %s | %d\n", u.id, u.name, u.age);
    }
    fclose(file);
}

void updateUser(){
    struct User u;
    int id, found = 0;

    printf("\nUpdating User Records:\n");
    printf("Enter ID to update: ");
    scanf("%d", &id);

    FILE *file = fopen("userRecords.txt", "r");
    FILE *temp = fopen("tempRecords.txt", "w");

    if (!file || !temp){
        printf("Error file opening!\n");
        if(file) fclose(file);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(file, "%d | %[^|] | %d", &u.id, u.name, &u.age) != EOF){
        if (u.id == id){
            found = 1;
            printf("New Name: ");
            getchar();
            scanf("%[^\n]", u.name);
            printf("New Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d | %s | %d\n", u.id, u.name, u.age);
    }
    fclose(file);
    fclose(temp);

    remove("userRecords.txt");
    rename("tempRecords.txt", "userRecords.txt");

    if (!found){
        printf("User Record not found.\n");
    }
    else{
        printf("\nUser Record updated successfully.\n");
    }
}

void deleteUser(){
    struct User u;
    int id, found = 0;

    printf("\nDeleting User Record:\n");
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *file = fopen("userRecords.txt", "r");
    FILE *temp = fopen("tempRecords.txt", "w");

    if (!file || !temp){
        printf("Error file opening!\n");
        if(file) fclose(file);
        if(temp) fclose(temp);
        return;
    }

    while (fscanf(file, "%d | %[^|] | %d", &u.id, u.name, &u.age) != EOF){
        if (u.id == id){
            found = 1;
            continue;  // skip this user
        }
        fprintf(temp, "%d | %s | %d\n", u.id, u.name, u.age);
    }
    fclose(file);
    fclose(temp);

    remove("userRecords.txt");
    rename("tempRecords.txt", "userRecords.txt");

    if (!found){
        printf("User not found.\n");
    }
    else{
        printf("\nUser deleted successfully.\n");
    }
}

int main() {
    int op;
    do {
        printf("\n\nUsers Records, perform the below operations");
        printf("\n1. Create\n2. Read\n3. Update\n4. Delete\n5. Exit\n\nChoice: ");
        scanf("%d", &op);

        if (op == 1)
            createUser();
        else if (op == 2)
            readUsers();
        else if (op == 3)
            updateUser();
        else if (op == 4)
            deleteUser();
        else if (op == 5)
            printf("Exiting...\n");
        else
            printf("Invalid choice, choose between 1 to 5!\n");

    } while (op != 5);

    return 0;
}
