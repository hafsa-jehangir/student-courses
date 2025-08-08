#include <stdio.h>
#include <string.h>
#define MAX_COURSES 5

// Structure to store student information
struct Student {
    char name[50];
    char password[20];
    char courses[MAX_COURSES][30];
    int courseCount;
};

struct Student student;
char currentFilename[100];  // Holds the filename for current user data

// Function declarations
void signup();
int login();
void showMenu();
void addCourse();
void dropCourse();
void viewCourses();
void searchCourse();
void updateCourse();
void printCoursesRecursively(int index);
void showProfile(struct Student *s);
void saveToFile();
int loadFromFile(const char *filename);

// Entry Point
int main() {
    int success = 0;

    printf("\n=====================================\n");
    printf("         STUDENT PORTAL          \n");
    printf("=====================================\n");
    printf("Enter your name: ");
    scanf(" %[^\n]", student.name);
    getchar();

    // Make file name like Hafsa.txt
    sprintf(currentFilename, "%s.txt", student.name);

    int loaded = loadFromFile(currentFilename);

    if (!loaded) {
        printf("No previous data found. Please signup.\n");
        signup();
    }

    success = login();
    if (success) {
        showMenu();
    } else {
        printf("\nLogin failed. Exiting...\n");
    }

    return 0;
}

// Signup function
void signup() {
    printf("Set your password: ");
    scanf(" %[^\n]", student.password);
    getchar();
    student.courseCount = 0;
    printf("Signup successful!\n");
    saveToFile();
}

// Login function
int login() {
    char enteredPassword[20];

    printf("\n=====================================\n");
    printf("             LOGIN               \n");
    printf("=====================================\n");
    printf("Enter your password: ");
    scanf(" %[^\n]", enteredPassword);
    getchar();

    if (strcmp(enteredPassword, student.password) == 0) {
        printf("Login successful. Welcome, %s!\n", student.name);
        return 1;
    } else {
        return 0;
    }
}

// Main Menu
void showMenu() {
    int choice;
    do {
        printf("\n=====================================\n");
        printf("             MAIN MENU            \n");
        printf("=====================================\n");
        printf("2. View Profile\n");
        printf("3. Add New Course\n");
        printf("4. Drop Course\n");
        printf("5. Search Course\n");
        printf("6. View All Courses\n");
        printf("7. Exit and save data\n");
        printf("8. Update Course\n");
        printf("-------------------------------------\n");
        printf("Enter your choice (2–8): ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 2:
                printf("\n-------- PROFILE --------\n");
                showProfile(&student);
                break;
            case 3:
                addCourse();
                break;
            case 4:
                dropCourse();
                break;
            case 5:
                searchCourse();
                break;
            case 6:
                viewCourses();
                break;
            case 7:
                saveToFile();
                printf("Data saved. Goodbye, %s!\n", student.name);
                break;
            case 8:
                updateCourse();
                break;
            default:
                printf("Invalid choice. Please select 2–8.\n");
        }
    } while (choice != 7);
}

// Add a course
void addCourse() {
    if (student.courseCount >= MAX_COURSES) {
        printf("You can't add more than %d courses.\n", MAX_COURSES);
        return;
    }
    printf("Enter course name to add: ");
    scanf(" %[^\n]", student.courses[student.courseCount]);
    getchar();
    student.courseCount++;
    printf("Course added successfully.\n");
    saveToFile();
}

// Drop a course
void dropCourse() {
    char drop[30];
    int found = 0;
    printf("Enter course name to drop: ");
    scanf(" %[^\n]", drop);
    getchar();

    for (int i = 0; i < student.courseCount; i++) {
        if (strcmp(drop, student.courses[i]) == 0) {
            for (int j = i; j < student.courseCount - 1; j++) {
                strcpy(student.courses[j], student.courses[j + 1]);
            }
            student.courseCount--;
            found = 1;
            printf("Course dropped.\n");
            break;
        }
    }
    if (!found) {
        printf("Course not found.\n");
    }
    saveToFile();
}

// View all courses
void viewCourses() {
    printf("\nEnrolled Courses:\n");
    printf("---------------------\n");
    if (student.courseCount == 0) {
        printf("No courses added yet.\n");
    } else {
        printCoursesRecursively(0);
    }
}

// Recursive course printer
void printCoursesRecursively(int index) {
    if (index == student.courseCount) return;
    printf("Course %d: %s\n", index + 1, student.courses[index]);
    printCoursesRecursively(index + 1);
}

// Search for a course
void searchCourse() {
    char search[30];
    int found = 0;
    printf("Enter keyword to search in courses: ");
    scanf(" %[^\n]", search);
    getchar();

    for (int i = 0; i < student.courseCount; i++) {
        if (strstr(student.courses[i], search)) {
            printf("Found: %s\n", student.courses[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No matching course found.\n");
    }
}

// Update a course name
void updateCourse() {
    char oldName[30];
    int found = 0;
    printf("Enter the course name to update: ");
    scanf(" %[^\n]", oldName);
    getchar();

    for (int i = 0; i < student.courseCount; i++) {
        if (strcmp(oldName, student.courses[i]) == 0) {
            printf("Enter new course name: ");
            scanf(" %[^\n]", student.courses[i]);
            getchar();
            printf("Course updated.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Course not found.\n");
    }
    saveToFile();
}

// Show student profile using pointer
void showProfile(struct Student *s) {
    printf("Name     : %s\n", s->name);
    printf("Password : %s\n", s->password);
    printf("Courses Enrolled : %d\n", s->courseCount);
}

// Save data to file
void saveToFile() {
    FILE *f = fopen(currentFilename, "w");
    if (f == NULL) {
        printf("Error saving file.\n");
        return;
    }

    fprintf(f, "Name: %s\n", student.name);
    fprintf(f, "Password: %s\n", student.password);
    fprintf(f, "Total Courses: %d\n", student.courseCount);

    for (int i = 0; i < student.courseCount; i++) {
        fprintf(f, "Course %d: %s\n", i + 1, student.courses[i]);
    }

    fclose(f);
}

// Load data from file  
int loadFromFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return 0;

    fscanf(f, "Name: %[^\n]\n", student.name);
    fscanf(f, "Password: %[^\n]\n", student.password);
    fscanf(f, "Total Courses: %d\n", &student.courseCount);

    for (int i = 0; i < student.courseCount; i++) {
        fscanf(f, "Course %*d: %[^\n]\n", student.courses[i]);
    }

    fclose(f);
    return 1;
}
