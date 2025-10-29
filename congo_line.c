/*
 * Congo Line Management System using Linked List
 * This program simulates a Congo line (dance line) where people can join,
 * leave, and move positions in the line using a linked list structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a person in the Congo line
typedef struct Person {
    char name[50];
    int id;
    struct Person* next;
} Person;

// Structure to represent the Congo line
typedef struct CongoLine {
    Person* head;    // Leader of the line
    Person* tail;    // Last person in the line
    int totalPeople;
} CongoLine;

// Function to create a new Congo line
CongoLine* createCongoLine() {
    CongoLine* line = (CongoLine*)malloc(sizeof(CongoLine));
    line->head = NULL;
    line->tail = NULL;
    line->totalPeople = 0;
    return line;
}

// Function to create a new person node
Person* createPerson(char* name, int id) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    strcpy(newPerson->name, name);
    newPerson->id = id;
    newPerson->next = NULL;
    return newPerson;
}

// Function to join the Congo line at the end
void joinLine(CongoLine* line, char* name, int id) {
    Person* newPerson = createPerson(name, id);
    
    if (line->head == NULL) {
        // First person becomes the leader
        line->head = newPerson;
        line->tail = newPerson;
        printf("%s (ID: %d) started the Congo line as the leader!\n", name, id);
    } else {
        // Add to the end of the line
        line->tail->next = newPerson;
        line->tail = newPerson;
        printf("%s (ID: %d) joined the Congo line!\n", name, id);
    }
    
    line->totalPeople++;
}

// Function to leave the Congo line
void leaveLine(CongoLine* line, int id) {
    if (line->head == NULL) {
        printf("Congo line is empty!\n");
        return;
    }
    
    Person* current = line->head;
    Person* previous = NULL;
    
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Person with ID %d not found in the line!\n", id);
        return;
    }
    
    // Found the person to remove
    if (previous == NULL) {
        // Removing the leader (head)
        line->head = current->next;
        if (line->head == NULL) {
            line->tail = NULL;
        }
        printf("%s (ID: %d) left the line. ", current->name, current->id);
        if (line->head != NULL) {
            printf("%s is now the new leader!\n", line->head->name);
        } else {
            printf("Congo line is now empty!\n");
        }
    } else {
        // Removing someone in the middle or at the end
        previous->next = current->next;
        if (current == line->tail) {
            line->tail = previous;
        }
        printf("%s (ID: %d) left the Congo line!\n", current->name, current->id);
    }
    
    free(current);
    line->totalPeople--;
}

// Function to move a person to the front (become leader)
void moveToFront(CongoLine* line, int id) {
    if (line->head == NULL) {
        printf("Congo line is empty!\n");
        return;
    }
    
    if (line->head->id == id) {
        printf("%s is already the leader!\n", line->head->name);
        return;
    }
    
    Person* current = line->head;
    Person* previous = NULL;
    
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Person with ID %d not found in the line!\n", id);
        return;
    }
    
    // Remove from current position
    previous->next = current->next;
    if (current == line->tail) {
        line->tail = previous;
    }
    
    // Move to front
    current->next = line->head;
    line->head = current;
    
    printf("%s (ID: %d) moved to the front and is now the leader!\n", 
           current->name, current->id);
}

// Function to swap two adjacent people in the line
void swapAdjacent(CongoLine* line, int id1, int id2) {
    if (line->head == NULL || line->head->next == NULL) {
        printf("Need at least 2 people to swap!\n");
        return;
    }
    
    Person* current = line->head;
    Person* previous = NULL;
    
    // Find the first person and check if second is adjacent
    while (current != NULL && current->next != NULL) {
        if ((current->id == id1 && current->next->id == id2) ||
            (current->id == id2 && current->next->id == id1)) {
            
            Person* first = current;
            Person* second = current->next;
            
            // Perform the swap
            first->next = second->next;
            second->next = first;
            
            if (previous == NULL) {
                // Swapping at the head
                line->head = second;
            } else {
                previous->next = second;
            }
            
            // Update tail if necessary
            if (first->next == NULL) {
                line->tail = first;
            }
            
            printf("Swapped %s and %s in the Congo line!\n", 
                   first->name, second->name);
            return;
        }
        
        previous = current;
        current = current->next;
    }
    
    printf("People with IDs %d and %d are not adjacent or not found!\n", id1, id2);
}

// Function to display the Congo line
void displayLine(CongoLine* line) {
    if (line->head == NULL) {
        printf("Congo line is empty!\n");
        return;
    }
    
    printf("\n=== CONGO LINE ===\n");
    printf("Leader -> ");
    
    Person* current = line->head;
    int position = 1;
    
    while (current != NULL) {
        printf("%s (ID: %d)", current->name, current->id);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
        position++;
    }
    
    printf(" <- End\n");
    printf("Total people in line: %d\n\n", line->totalPeople);
}

// Function to find position of a person in the line
int findPosition(CongoLine* line, int id) {
    Person* current = line->head;
    int position = 1;
    
    while (current != NULL) {
        if (current->id == id) {
            return position;
        }
        current = current->next;
        position++;
    }
    
    return -1; // Not found
}

// Function to start the Congo dance (simulate movement)
void startDance(CongoLine* line) {
    if (line->head == NULL) {
        printf("Cannot start dance - Congo line is empty!\n");
        return;
    }
    
    printf("\n🎵 CONGO DANCE STARTED! 🎵\n");
    printf("The Congo line is dancing:\n");
    
    Person* current = line->head;
    while (current != NULL) {
        printf("%s is dancing! ", current->name);
        if (current == line->head) {
            printf("(Leading the dance)");
        }
        printf("\n");
        current = current->next;
    }
    
    printf("🎵 Great dancing everyone! 🎵\n\n");
}

// Function to free the Congo line memory
void freeCongoLine(CongoLine* line) {
    Person* current = line->head;
    while (current != NULL) {
        Person* next = current->next;
        free(current);
        current = next;
    }
    free(line);
}

// Main function with menu-driven interface
int main() {
    CongoLine* congoLine = createCongoLine();
    int choice, id, id2, position;
    char name[50];
    
    printf("Welcome to the Congo Line Manager!\n");
    printf("Get ready to dance! 💃🕺\n");
    
    do {
        printf("\n=== CONGO LINE MENU ===\n");
        printf("1. Join the line\n");
        printf("2. Leave the line\n");
        printf("3. Display line\n");
        printf("4. Move to front (become leader)\n");
        printf("5. Swap two adjacent people\n");
        printf("6. Find person's position\n");
        printf("7. Start Congo dance\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; // remove newline
                
                printf("Enter ID: ");
                scanf("%d", &id);
                
                joinLine(congoLine, name, id);
                break;
                
            case 2:
                printf("Enter ID of person leaving: ");
                scanf("%d", &id);
                
                leaveLine(congoLine, id);
                break;
                
            case 3:
                displayLine(congoLine);
                break;
                
            case 4:
                printf("Enter ID of person to move to front: ");
                scanf("%d", &id);
                
                moveToFront(congoLine, id);
                break;
                
            case 5:
                printf("Enter first person's ID: ");
                scanf("%d", &id);
                printf("Enter second person's ID: ");
                scanf("%d", &id2);
                
                swapAdjacent(congoLine, id, id2);
                break;
                
            case 6:
                printf("Enter ID to find: ");
                scanf("%d", &id);
                
                position = findPosition(congoLine, id);
                if (position != -1) {
                    printf("Person with ID %d is at position %d\n", id, position);
                } else {
                    printf("Person with ID %d not found in the line\n", id);
                }
                break;
                
            case 7:
                startDance(congoLine);
                break;
                
            case 8:
                printf("Thanks for dancing! The Congo line is ending.\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
    
    freeCongoLine(congoLine);
    return 0;
}