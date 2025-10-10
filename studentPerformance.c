#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 3

struct Student {
    int rollNo;
    char name[50];
    float marks[3];
};

// Function prototypes
void inputStudentData(struct Student students[], int numOfStudents);
float totalMarks(struct Student student);
float totalAverage(struct Student student);
char calculateGrade(float average);
void printPattern(char grade);
void printStudentData(struct Student students[], int numOfStudents);
void printRollNumbers(struct Student students[], int index, int numOfStudents);
void runProgram();


void inputStudentData(struct Student students[], int numOfStudents) {
    for (int i = 0; i < numOfStudents; i++) {
        printf("\nEnter details for Student %d\n", i + 1);
        printf("Enter Roll Number: ");
        scanf("%d", &students[i].rollNo);

        printf("Enter Name: ");
        scanf("%s", students[i].name);

        printf("Enter marks in 3 subjects: ");
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            scanf("%f", &students[i].marks[j]);
        }
    }
}

float totalMarks(struct Student student) {
    float total = 0;

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total += student.marks[i];
    }

    return total;
}

float totalAverage(struct Student student) {
    return totalMarks(student) / MAX_SUBJECTS;
}

char calculateGrade(float average) {
    if (average >= 85) {
        return 'A';
    } else if (average >= 70) {
        return 'B';
    } else if (average >= 50) {
        return 'C';
    } else if (average >= 35) {
        return 'D';
    } else {
        return 'F';
    }
}

void printPattern(char grade) {

    int stars = 0;
    switch (grade) {
        case 'A': 
            stars = 5; 
            break;
        case 'B': 
            stars = 4; 
            break;
        case 'C': 
            stars = 3; 
            break;
        case 'D': 
            stars = 2; 
            break;
        default: 
            stars = 0; 
            break;
    }

    printf("Performance: ");
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
    printf("\n");
}


void printStudentData(struct Student students[], int numOfStudents) {
    for (int i = 0; i < numOfStudents; i++) {
        printf("\nRoll: %d\n", students[i].rollNo);
        printf("Name: %s\n", students[i].name);

        float total = totalMarks(students[i]);
        float average = totalAverage(students[i]);
        char grade = calculateGrade(average);

        printf("Total: %.2f\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);

        // Skip stars if grade is F
        if (grade == 'F') {
            continue;
        }
        else{
            printPattern(grade);
        }
    }
}

// Recursive function to print roll numbers
void printRollNumbers(struct Student students[], int index, int numOfStudents) {
    if (index == numOfStudents) {
        return;
    }
    printf("%d ", students[index].rollNo);
    printRollNumbers(students, index + 1, numOfStudents);
}

void runProgram() {
    struct Student students[MAX_STUDENTS];
    int numOfStudents;

    printf("Enter number of students: ");
    scanf("%d", &numOfStudents);

    if (numOfStudents <= 0 || numOfStudents > MAX_STUDENTS) {
    printf("Invalid number of students!\n");
    return;
}


    inputStudentData(students, numOfStudents);
    printStudentData(students, numOfStudents);

    printf("\nRoll Numbers of all students: ");
    printRollNumbers(students, 0, numOfStudents);
    printf("\n");
}

int main() {
    runProgram();
    return 0;
}
