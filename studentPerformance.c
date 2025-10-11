#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int MAX_STUDENTS = 100;
const int MAX_SUBJECTS = 3;
const int MAX_MARKS = 100;
const int MIN_MARKS = 0;

typedef struct {
    int rollNo;
    char name[50];
    float marks[3];
} Student;

bool isDuplicateRoll(const Student students[], int count, int rollNo) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) {
            return true;
        }
    }
    return false;
}

float calculateTotalMarks(const Student student) {
    float total = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total += student.marks[i];
    }
    return total;
}

float calculateAverage(const Student student) {
    return calculateTotalMarks(student) / MAX_SUBJECTS;
}

char calculateGrade(float average) {
    if (average >= 85) {
        return 'A';
    }
    else if (average >= 70) {
        return 'B';
    }
    else if (average >= 50) {
        return 'C';
    }
    else if (average >= 35) {
        return 'D';
    }
    else {
        return 'F';
    }
}

void printPerformancePattern(char grade) {
    int stars = 0;

    if (grade == 'A') {
        stars = 5;
    }
    else if (grade == 'B') {
        stars = 4;
    }
    else if (grade == 'C') {
        stars = 3;
    }
    else if (grade == 'D') {
        stars = 2;
    }
    else {
        stars = 0;
    }

    if (stars > 0) {
        printf("Performance: ");
        for (int i = 0; i < stars; i++) {
            printf("*");
        }
        printf("\n");
    }
}

void printStudentData(const Student students[], int numOfStudents) {

    for (int i = 0; i < numOfStudents; i++) {

        if (students[i].rollNo == 0) {
            continue;
        }

        const float total = calculateTotalMarks(students[i]);
        const float average = calculateAverage(students[i]);
        const char grade = calculateGrade(average);

        printf("\nRoll: %d\n", students[i].rollNo);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.2f\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);

        if (grade == 'F') {
            continue;
        }
        else{
            printPerformancePattern(grade);
        }
    }
}

void printRollNumbers(const Student students[], int index, int numOfStudents) {
    if (index == numOfStudents) {
        return;
    }
    if (students[index].rollNo != 0){
        printf("%d ", students[index].rollNo);
    }
    printRollNumbers(students, index + 1, numOfStudents);
}

void sortStudentsByRollNo(Student students[], int numOfStudents) {
    for (int i = 0; i < numOfStudents - 1; i++) {
        for (int j = i + 1; j < numOfStudents; j++) {
            if (students[i].rollNo > students[j].rollNo) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}


void inputStudentData(Student students[], int *numOfStudents) {
    
    if (*numOfStudents > MAX_STUDENTS) {
        printf("Number of students exceeds maximum limit (%d). Setting to max.\n", MAX_STUDENTS);
        *numOfStudents = MAX_STUDENTS;
    }

    for (int i = 0; i < *numOfStudents; i++) {
       
        int rollNo;
        scanf("%d", &rollNo);

        if (isDuplicateRoll(students, i, rollNo)) {
            printf("Duplicate roll number found. Skipping entry.\n");
            students[i].rollNo = 0; 
            continue;
        }

        students[i].rollNo = rollNo;

        scanf("%s", students[i].name);

        for (int j = 0; j < MAX_SUBJECTS; j++) {
            do {
                
                scanf("%f", &students[i].marks[j]);

                if (students[i].marks[j] < MIN_MARKS || students[i].marks[j] > MAX_MARKS) {
                    printf(" Invalid marks! Please enter a value between 0 and 100.\n");
                }

            } while (students[i].marks[j] < MIN_MARKS || students[i].marks[j] > MAX_MARKS);
        }
    }
}

void studentPerformance() {
    Student students[MAX_STUDENTS];

    int numOfStudents = 0;
    scanf("%d", &numOfStudents);

    inputStudentData(students, &numOfStudents);

    sortStudentsByRollNo(students, numOfStudents);

    printStudentData(students, numOfStudents);

    printf("\nList of Roll Numbers (via recursion): ");
    int startIndex = 0;
    printRollNumbers(students, startIndex, numOfStudents);
    printf("\n");
}

int main() {
    studentPerformance();
    return 0;
}
