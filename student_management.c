#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"

// Structure for storing data in file (without pointer)
typedef struct {
    int id;
    char name[50];
    float fees;
    int feesPaid;       // 0 = Unpaid, 1 = Paid
    char course[10];    // DS, AI, ML, Web, or None
    char grade[15];     // Excellent, Good, Fail, or Pending
} StudentRecord;

// Linked List Node Structure
typedef struct Node {
    StudentRecord data;
    struct Node* next;
} Node;

Node* head = NULL;

// Function Prototypes
int authenticate();
void loadFromFile();
void saveToFile();
void registerStudent();
void payFeesAndAllotCourse();
void declareResult();
void displayAllStudents();
void searchStudent();
void deleteStudent();
void freeList();
Node* findStudentById(int id);
void clearInputBuffer();

int main() {
    if (!authenticate()) {
        printf("\n[ACCESS DENIED] Too many failed login attempts. Exiting...\n");
        return 1;
    }

    loadFromFile();

    int choice;
    while (1) {
        printf("\n==================================================\n");
        printf("   INSTITUTE PERSISTENT STUDENT MANAGEMENT SYSTEM \n");
        printf("==================================================\n");
        printf("1. Register New Student\n");
        printf("2. Pay Fees & Allot Course (DS / AI / ML / Web)\n");
        printf("3. Declare Course Result (Excellent / Good / Fail)\n");
        printf("4. Display All Students\n");
        printf("5. Search Student by ID\n");
        printf("6. Delete Student Record\n");
        printf("7. Save & Exit\n");
        printf("--------------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n[!] Invalid input. Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: payFeesAndAllotCourse(); break;
            case 3: declareResult(); break;
            case 4: displayAllStudents(); break;
            case 5: searchStudent(); break;
            case 6: deleteStudent(); break;
            case 7:
                saveToFile();
                freeList();
                printf("\n[SUCCESS] All data saved to '%s'. Goodbye!\n", DATA_FILE);
                return 0;
            default:
                printf("\n[!] Invalid choice. Please select between 1 and 7.\n");
        }
    }
    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Authentication System
int authenticate() {
    char userId[30], password[30];
    const char validUser[] = "admin";
    const char validPass[] = "admin123";
    int attempts = 3;

    printf("==================================================\n");
    printf("               SYSTEM AUTHENTICATION              \n");
    printf("==================================================\n");

    while (attempts > 0) {
        printf("User ID  : ");
        scanf("%29s", userId);
        printf("Password : ");
        scanf("%29s", password);
        clearInputBuffer();

        if (strcmp(userId, validUser) == 0 && strcmp(password, validPass) == 0) {
            printf("\n[SUCCESS] Login successful! Welcome, %s.\n", userId);
            return 1;
        } else {
            attempts--;
            printf("[ERROR] Invalid credentials. Attempts left: %d\n\n", attempts);
        }
    }
    return 0;
}

// Load persistent data from binary file into Linked List
void loadFromFile() {
    FILE* fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        return; // File doesn't exist yet on first run
    }

    StudentRecord rec;
    Node* tail = NULL;

    while (fread(&rec, sizeof(StudentRecord), 1, fp) == 1) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) {
            printf("[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }
        newNode->data = rec;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(fp);
}

// Save Linked List data to binary file
void saveToFile() {
    FILE* fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("[ERROR] Could not open file for saving!\n");
        return;
    }

    Node* temp = head;
    while (temp != NULL) {
        fwrite(&(temp->data), sizeof(StudentRecord), 1, fp);
        temp = temp->next;
    }
    fclose(fp);
}

Node* findStudentById(int id) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->data.id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 1. Register Student
void registerStudent() {
    int id;
    printf("\nEnter Student ID (numeric): ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("[!] Invalid ID format.\n");
        return;
    }
    clearInputBuffer();

    if (findStudentById(id) != NULL) {
        printf("[!] A student with ID %d already exists.\n", id);
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    newNode->data.id = id;
    printf("Enter Student Name: ");
    fgets(newNode->data.name, sizeof(newNode->data.name), stdin);
    newNode->data.name[strcspn(newNode->data.name, "\n")] = '\0'; // Remove newline

    newNode->data.fees = 0.0f;
    newNode->data.feesPaid = 0;
    strcpy(newNode->data.course, "None");
    strcpy(newNode->data.grade, "Pending");
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    saveToFile();
    printf("[SUCCESS] Student '%s' (ID: %d) registered successfully.\n", newNode->data.name, id);
}

// 2. Pay Fees & Allot Course
void payFeesAndAllotCourse() {
    int id, cChoice;
    float amount;

    printf("\nEnter Student ID: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("[!] Invalid ID.\n");
        return;
    }
    clearInputBuffer();

    Node* student = findStudentById(id);
    if (!student) {
        printf("[!] Student with ID %d not found.\n", id);
        return;
    }

    printf("Enter Fee Amount Paid (INR): ");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("[!] Invalid fee amount.\n");
        return;
    }
    clearInputBuffer();

    student->data.fees = amount;
    student->data.feesPaid = 1;

    printf("\nSelect Course to Allot:\n");
    printf("1. DS (Data Science)\n");
    printf("2. AI (Artificial Intelligence)\n");
    printf("3. ML (Machine Learning)\n");
    printf("4. Web (Web Development)\n");
    printf("Choice (1-4): ");
    scanf("%d", &cChoice);
    clearInputBuffer();

    switch (cChoice) {
        case 1: strcpy(student->data.course, "DS"); break;
        case 2: strcpy(student->data.course, "AI"); break;
        case 3: strcpy(student->data.course, "ML"); break;
        case 4: strcpy(student->data.course, "Web"); break;
        default:
            printf("[!] Invalid course choice. Defaulting to 'DS'.\n");
            strcpy(student->data.course, "DS");
    }

    saveToFile();
    printf("[SUCCESS] Fees (Rs. %.2f) recorded and course '%s' allotted to %s.\n",
           student->data.fees, student->data.course, student->data.name);
}

// 3. Declare Result
void declareResult() {
    int id, gChoice;
    printf("\nEnter Student ID: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("[!] Invalid ID.\n");
        return;
    }
    clearInputBuffer();

    Node* student = findStudentById(id);
    if (!student) {
        printf("[!] Student with ID %d not found.\n", id);
        return;
    }

    if (!student->data.feesPaid) {
        printf("[!] Fees not paid yet. Please pay fees and allot a course first.\n");
        return;
    }

    printf("\nSelect Grade for %s (Course: %s):\n", student->data.name, student->data.course);
    printf("1. Excellent\n");
    printf("2. Good\n");
    printf("3. Fail\n");
    printf("Choice (1-3): ");
    scanf("%d", &gChoice);
    clearInputBuffer();

    switch (gChoice) {
        case 1: strcpy(student->data.grade, "Excellent"); break;
        case 2: strcpy(student->data.grade, "Good"); break;
        case 3: strcpy(student->data.grade, "Fail"); break;
        default:
            printf("[!] Invalid grade choice.\n");
            return;
    }

    saveToFile();
    printf("[SUCCESS] Result '%s' declared for %s.\n", student->data.grade, student->data.name);
}

// 4. Display All Students
void displayAllStudents() {
    if (head == NULL) {
        printf("\n[INFO] No student records found.\n");
        return;
    }

    printf("\n==============================================================================\n");
    printf("%-6s %-22s %-12s %-10s %-10s %-12s\n", "ID", "Name", "Fees (INR)", "Status", "Course", "Grade");
    printf("------------------------------------------------------------------------------\n");

    Node* temp = head;
    while (temp != NULL) {
        printf("%-6d %-22s %-12.2f %-10s %-10s %-12s\n",
               temp->data.id,
               temp->data.name,
               temp->data.fees,
               temp->data.feesPaid ? "Paid" : "Unpaid",
               temp->data.course,
               temp->data.grade);
        temp = temp->next;
    }
    printf("==============================================================================\n");
}

// 5. Search Student by ID
void searchStudent() {
    int id;
    printf("\nEnter Student ID to search: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("[!] Invalid ID.\n");
        return;
    }
    clearInputBuffer();

    Node* s = findStudentById(id);
    if (!s) {
        printf("[!] No student found with ID %d.\n", id);
        return;
    }

    printf("\n--- Student Details ---\n");
    printf("ID          : %d\n", s->data.id);
    printf("Name        : %s\n", s->data.name);
    printf("Fees Paid   : Rs. %.2f (%s)\n", s->data.fees, s->data.feesPaid ? "Paid" : "Unpaid");
    printf("Course      : %s\n", s->data.course);
    printf("Final Grade : %s\n", s->data.grade);
}

// 6. Delete Student Record
void deleteStudent() {
    int id;
    printf("\nEnter Student ID to delete: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("[!] Invalid ID.\n");
        return;
    }
    clearInputBuffer();

    Node *temp = head, *prev = NULL;
    while (temp != NULL && temp->data.id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("[!] Student with ID %d not found.\n", id);
        return;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    saveToFile();
    printf("[SUCCESS] Student with ID %d deleted successfully.\n", id);
}

// Free allocated memory
void freeList() {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
