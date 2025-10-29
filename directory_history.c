/*
 * Directory History Navigation System using Doubly Linked List
 * This program implements a file directory history system that allows
 * users to navigate back and forth through visited directories.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a directory entry in history
typedef struct DirectoryEntry {
    char path[256];
    char dirName[100];
    time_t visitTime;
    int visitCount;
    struct DirectoryEntry* prev;
    struct DirectoryEntry* next;
} DirectoryEntry;

// Structure to represent the directory history system
typedef struct DirectoryHistory {
    DirectoryEntry* head;        // Oldest entry
    DirectoryEntry* tail;        // Newest entry
    DirectoryEntry* current;     // Current directory
    int totalEntries;
    int maxHistorySize;
} DirectoryHistory;

// Function to create a new directory entry
DirectoryEntry* createDirectoryEntry(char* path, char* dirName) {
    DirectoryEntry* newEntry = (DirectoryEntry*)malloc(sizeof(DirectoryEntry));
    strcpy(newEntry->path, path);
    strcpy(newEntry->dirName, dirName);
    newEntry->visitTime = time(NULL);
    newEntry->visitCount = 1;
    newEntry->prev = NULL;
    newEntry->next = NULL;
    return newEntry;
}

// Function to create directory history system
DirectoryHistory* createDirectoryHistory(int maxSize) {
    DirectoryHistory* history = (DirectoryHistory*)malloc(sizeof(DirectoryHistory));
    history->head = NULL;
    history->tail = NULL;
    history->current = NULL;
    history->totalEntries = 0;
    history->maxHistorySize = maxSize;
    return history;
}

// Function to find if directory already exists in history
DirectoryEntry* findDirectory(DirectoryHistory* history, char* path) {
    DirectoryEntry* current = history->head;
    while (current != NULL) {
        if (strcmp(current->path, path) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to remove oldest entry when history is full
void removeOldestEntry(DirectoryHistory* history) {
    if (history->head == NULL) return;
    
    DirectoryEntry* oldHead = history->head;
    
    if (history->head == history->tail) {
        // Only one entry
        history->head = NULL;
        history->tail = NULL;
        history->current = NULL;
    } else {
        history->head = history->head->next;
        history->head->prev = NULL;
        
        // Update current if it was pointing to removed entry
        if (history->current == oldHead) {
            history->current = history->head;
        }
    }
    
    printf("Removed oldest entry: %s\n", oldHead->dirName);
    free(oldHead);
    history->totalEntries--;
}

// Function to move entry to tail (most recent)
void moveToTail(DirectoryHistory* history, DirectoryEntry* entry) {
    if (entry == history->tail) {
        // Already at tail
        return;
    }
    
    // Remove from current position
    if (entry->prev) {
        entry->prev->next = entry->next;
    } else {
        history->head = entry->next;
    }
    
    if (entry->next) {
        entry->next->prev = entry->prev;
    }
    
    // Add to tail
    entry->prev = history->tail;
    entry->next = NULL;
    history->tail->next = entry;
    history->tail = entry;
}

// Function to visit a directory (add to history or update existing)
void visitDirectory(DirectoryHistory* history, char* path, char* dirName) {
    DirectoryEntry* existing = findDirectory(history, path);
    
    if (existing != NULL) {
        // Directory already in history - update and move to end
        existing->visitTime = time(NULL);
        existing->visitCount++;
        moveToTail(history, existing);
        history->current = existing;
        printf("Revisited: %s (visit count: %d)\n", dirName, existing->visitCount);
    } else {
        // New directory - add to history
        DirectoryEntry* newEntry = createDirectoryEntry(path, dirName);
        
        // Check if history is full
        if (history->totalEntries >= history->maxHistorySize) {
            removeOldestEntry(history);
        }
        
        // Add to tail
        if (history->head == NULL) {
            history->head = newEntry;
            history->tail = newEntry;
        } else {
            history->tail->next = newEntry;
            newEntry->prev = history->tail;
            history->tail = newEntry;
        }
        
        history->current = newEntry;
        history->totalEntries++;
        printf("Visited new directory: %s\n", dirName);
    }
}

// Function to go back to previous directory
void goBack(DirectoryHistory* history) {
    if (history->current == NULL) {
        printf("No current directory!\n");
        return;
    }
    
    if (history->current->prev == NULL) {
        printf("Already at the oldest directory in history!\n");
        return;
    }
    
    history->current = history->current->prev;
    printf("Moved back to: %s\n", history->current->dirName);
    printf("Path: %s\n", history->current->path);
}

// Function to go forward to next directory
void goForward(DirectoryHistory* history) {
    if (history->current == NULL) {
        printf("No current directory!\n");
        return;
    }
    
    if (history->current->next == NULL) {
        printf("Already at the newest directory in history!\n");
        return;
    }
    
    history->current = history->current->next;
    printf("Moved forward to: %s\n", history->current->dirName);
    printf("Path: %s\n", history->current->path);
}

// Function to display current directory info
void showCurrentDirectory(DirectoryHistory* history) {
    if (history->current == NULL) {
        printf("No current directory!\n");
        return;
    }
    
    DirectoryEntry* current = history->current;
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&current->visitTime));
    
    printf("\n=== CURRENT DIRECTORY ===\n");
    printf("Directory: %s\n", current->dirName);
    printf("Full Path: %s\n", current->path);
    printf("Last Visit: %s\n", timeStr);
    printf("Visit Count: %d\n", current->visitCount);
    
    // Show navigation options
    printf("\nNavigation:\n");
    if (current->prev) {
        printf("← Previous: %s\n", current->prev->dirName);
    } else {
        printf("← Previous: (none - oldest in history)\n");
    }
    
    if (current->next) {
        printf("→ Next: %s\n", current->next->dirName);
    } else {
        printf("→ Next: (none - newest in history)\n");
    }
    printf("\n");
}

// Function to display entire history
void showHistory(DirectoryHistory* history) {
    if (history->head == NULL) {
        printf("History is empty!\n");
        return;
    }
    
    printf("\n=== DIRECTORY HISTORY ===\n");
    printf("Total entries: %d (max: %d)\n\n", history->totalEntries, history->maxHistorySize);
    
    DirectoryEntry* current = history->head;
    int index = 1;
    
    while (current != NULL) {
        char timeStr[50];
        strftime(timeStr, sizeof(timeStr), "%m-%d %H:%M", localtime(&current->visitTime));
        
        printf("%d. %s", index, current->dirName);
        
        if (current == history->current) {
            printf(" [CURRENT]");
        }
        
        printf("\n   Path: %s\n", current->path);
        printf("   Visits: %d | Last: %s\n", current->visitCount, timeStr);
        
        if (current->next != NULL) {
            printf("   ↓\n");
        }
        
        current = current->next;
        index++;
    }
    printf("\n");
}

// Function to jump to a specific directory in history
void jumpToDirectory(DirectoryHistory* history, int index) {
    if (index < 1 || index > history->totalEntries) {
        printf("Invalid index! History has %d entries.\n", history->totalEntries);
        return;
    }
    
    DirectoryEntry* current = history->head;
    for (int i = 1; i < index; i++) {
        current = current->next;
    }
    
    history->current = current;
    printf("Jumped to: %s\n", current->dirName);
    printf("Path: %s\n", current->path);
}

// Function to search for directories in history
void searchHistory(DirectoryHistory* history, char* searchTerm) {
    if (history->head == NULL) {
        printf("History is empty!\n");
        return;
    }
    
    printf("\n=== SEARCH RESULTS for '%s' ===\n", searchTerm);
    DirectoryEntry* current = history->head;
    int found = 0;
    int index = 1;
    
    while (current != NULL) {
        if (strstr(current->dirName, searchTerm) != NULL || 
            strstr(current->path, searchTerm) != NULL) {
            
            char timeStr[50];
            strftime(timeStr, sizeof(timeStr), "%m-%d %H:%M", localtime(&current->visitTime));
            
            printf("%d. %s\n", index, current->dirName);
            printf("   Path: %s\n", current->path);
            printf("   Visits: %d | Last: %s\n", current->visitCount, timeStr);
            
            if (current == history->current) {
                printf("   [CURRENT DIRECTORY]\n");
            }
            printf("\n");
            found++;
        }
        current = current->next;
        index++;
    }
    
    if (found == 0) {
        printf("No directories found matching '%s'\n", searchTerm);
    } else {
        printf("Found %d matching directories.\n", found);
    }
    printf("\n");
}

// Function to clear history
void clearHistory(DirectoryHistory* history) {
    DirectoryEntry* current = history->head;
    while (current != NULL) {
        DirectoryEntry* next = current->next;
        free(current);
        current = next;
    }
    
    history->head = NULL;
    history->tail = NULL;
    history->current = NULL;
    history->totalEntries = 0;
    
    printf("History cleared!\n");
}

// Function to get most visited directories
void showMostVisited(DirectoryHistory* history, int count) {
    if (history->head == NULL) {
        printf("History is empty!\n");
        return;
    }
    
    // Create array for sorting
    DirectoryEntry* entries[history->totalEntries];
    DirectoryEntry* current = history->head;
    int index = 0;
    
    while (current != NULL) {
        entries[index++] = current;
        current = current->next;
    }
    
    // Simple bubble sort by visit count
    for (int i = 0; i < history->totalEntries - 1; i++) {
        for (int j = 0; j < history->totalEntries - i - 1; j++) {
            if (entries[j]->visitCount < entries[j + 1]->visitCount) {
                DirectoryEntry* temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }
    
    printf("\n=== TOP %d MOST VISITED DIRECTORIES ===\n", count);
    int limit = (count < history->totalEntries) ? count : history->totalEntries;
    
    for (int i = 0; i < limit; i++) {
        printf("%d. %s (visited %d times)\n", 
               i + 1, entries[i]->dirName, entries[i]->visitCount);
        printf("   Path: %s\n", entries[i]->path);
    }
    printf("\n");
}

// Function to free history memory
void freeDirectoryHistory(DirectoryHistory* history) {
    clearHistory(history);
    free(history);
}

// Main function with menu-driven interface
int main() {
    DirectoryHistory* dirHistory = createDirectoryHistory(20); // Max 20 entries
    int choice, index, count;
    char path[256], dirName[100], searchTerm[100];
    
    printf("📁 Welcome to Directory History Navigator! 📁\n");
    printf("Keep track of your directory visits and navigate easily!\n\n");
    
    // Add some sample directories
    visitDirectory(dirHistory, "/home/user", "user");
    visitDirectory(dirHistory, "/home/user/documents", "documents");
    visitDirectory(dirHistory, "/home/user/downloads", "downloads");
    visitDirectory(dirHistory, "/usr/local/bin", "bin");
    
    do {
        printf("\n=== DIRECTORY HISTORY MENU ===\n");
        printf("1. Visit directory\n");
        printf("2. Go back\n");
        printf("3. Go forward\n");
        printf("4. Show current directory\n");
        printf("5. Show full history\n");
        printf("6. Jump to directory (by index)\n");
        printf("7. Search history\n");
        printf("8. Show most visited\n");
        printf("9. Clear history\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter directory name: ");
                fgets(dirName, sizeof(dirName), stdin);
                dirName[strcspn(dirName, "\n")] = 0;
                
                printf("Enter full path: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                
                visitDirectory(dirHistory, path, dirName);
                break;
                
            case 2:
                goBack(dirHistory);
                break;
                
            case 3:
                goForward(dirHistory);
                break;
                
            case 4:
                showCurrentDirectory(dirHistory);
                break;
                
            case 5:
                showHistory(dirHistory);
                break;
                
            case 6:
                printf("Enter directory index (1-%d): ", dirHistory->totalEntries);
                scanf("%d", &index);
                jumpToDirectory(dirHistory, index);
                break;
                
            case 7:
                printf("Enter search term: ");
                fgets(searchTerm, sizeof(searchTerm), stdin);
                searchTerm[strcspn(searchTerm, "\n")] = 0;
                searchHistory(dirHistory, searchTerm);
                break;
                
            case 8:
                printf("How many top directories to show? ");
                scanf("%d", &count);
                showMostVisited(dirHistory, count);
                break;
                
            case 9:
                clearHistory(dirHistory);
                break;
                
            case 10:
                printf("Thanks for using Directory History Navigator!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 10);
    
    freeDirectoryHistory(dirHistory);
    return 0;
}