#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define FILENAME "todolist.txt"

struct TodoItem {
    int id;
    char title[100];
    char description[255];
    char dueDate[11];
    int status; // 0: Incomplete, 1: Complete
};

void addTask();
void viewTasks();
void markComplete();
void deleteTask();
void saveToFile();
void loadFromFile();
void displayMenu();

struct TodoItem tasks[100];  // Array to store up to 100 tasks
int taskCount = 0;  // Track the number of tasks

// Function to load tasks from the file
void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No saved tasks found.\n");
        return;
    }

    while (fscanf(file, "%d %[^\n] %[^\n] %s %d\n", &tasks[taskCount].id, tasks[taskCount].title, tasks[taskCount].description, tasks[taskCount].dueDate, &tasks[taskCount].status) != EOF) {
        taskCount++;
    }

    fclose(file);
}

// Function to save tasks to the file
void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for saving tasks.\n" );
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d\n%s\n%s\n%s\n%d\n", tasks[i].id, tasks[i].title, tasks[i].description, tasks[i].dueDate, tasks[i].status);
    }

    fclose(file);
}

// Function to display the list of tasks
void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n=========================================\n");
    printf("            Task List\n");
    printf("=========================================\n");
    printf("%-5s%-20s%-30s%-15s%-15s\n", "ID", "Title", "Description", "Due Date", "Status");
    printf("-----------------------------------------\n");

    for (int i = 0; i < taskCount; i++) {
        printf("%-5d%-20s%-30s%-15s%-15s\n", tasks[i].id, tasks[i].title, tasks[i].description, tasks[i].dueDate, 
               tasks[i].status == 0 ? "Incomplete" : "Complete");
    }

    printf("=========================================\n");
}

// Function to add a new task
void addTask() {
    struct TodoItem newTask;
    printf("\nEnter task title (max 100 characters): ");
    getchar();  // Clear the newline left by previous input
    fgets(newTask.title, sizeof(newTask.title), stdin);
    newTask.title[strcspn(newTask.title, "\n")] = '\0';  // Remove newline character

    printf("Enter task description (max 255 characters): ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    newTask.description[strcspn(newTask.description, "\n")] = '\0'; 

    printf("Enter task due date (DD/MM/YYYY): ");
    fgets(newTask.dueDate, sizeof(newTask.dueDate), stdin);
    newTask.dueDate[strcspn(newTask.dueDate, "\n")] = '\0'; // Remove newline character

    newTask.id = taskCount + 1;
    newTask.status = 0;  // Default to incomplete

    tasks[taskCount] = newTask;
    taskCount++;

    saveToFile();  // Save the updated list to file
    printf("Task added successfully!\n");
}

// Function to mark a task as complete
void markComplete() {
    int id;
    printf("Enter task ID to mark as complete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            tasks[i].status = 1;
            found = 1;
            break;
        }
    }

    if (found) {
        saveToFile();  // Save the updated list to file
        printf("Task marked as complete.\n");
    } else {
        printf("Task ID not found.\n");
    }
}

// Function to delete a task
void deleteTask() {
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];  // Shift tasks to fill the gap
            }
            taskCount--;
            found = 1;
            break;
        }
    }

    if (found) {
        saveToFile();  // Save the updated list to file
        printf("Task deleted.\n");
    } else {
        printf("Task ID not found.\n");
    }
}

// Function to display the menu and handle user choices
void displayMenu() {
    int choice;

    do {
        printf("\n=========================================\n");
        printf("           To-Do List Application\n" );
        printf("=========================================\n");
        printf("1. ""Add a Task" "\n");
        printf("2. "  "View All Tasks"  "\n");
        printf("3. " "Mark Task as Complete" "\n");
        printf("4. " "Delete a Task"  "\n");
        printf("5. " "Exit" "\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markComplete(); break;
            case 4: deleteTask(); break;
            case 5: printf("Exiting program...\n" ); break;
            default: printf("Invalid choice! Please try again.\n" );
        }
    } while (choice != 5);
}

int main() {
    loadFromFile();  // Load existing tasks from file when the program starts
    displayMenu();   // Show the menu to the user
    return 0;
}
