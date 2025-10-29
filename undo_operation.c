/*
 * Undo Operation System using Stack
 * This program implements an undo operation system similar to text editors
 * using stack data structure to track and reverse operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_SIZE 1000
#define MAX_OPERATIONS 100

// Enumeration for operation types
typedef enum {
    OP_INSERT,
    OP_DELETE,
    OP_REPLACE,
    OP_CLEAR,
    OP_APPEND
} OperationType;

// Structure to represent an operation
typedef struct Operation {
    int operationId;
    OperationType type;
    int position;
    char originalText[MAX_TEXT_SIZE];
    char newText[MAX_TEXT_SIZE];
    char deletedText[MAX_TEXT_SIZE];
    time_t timestamp;
    struct Operation* next;
} Operation;

// Structure to represent the undo stack
typedef struct UndoStack {
    Operation* top;
    int operationCount;
    int maxOperations;
} UndoStack;

// Structure to represent the text editor state
typedef struct TextEditor {
    char content[MAX_TEXT_SIZE];
    UndoStack* undoStack;
    UndoStack* redoStack;
    int nextOperationId;
    int cursorPosition;
} TextEditor;

// Function to get operation type string
const char* getOperationTypeString(OperationType type) {
    switch (type) {
        case OP_INSERT: return "INSERT";
        case OP_DELETE: return "DELETE";
        case OP_REPLACE: return "REPLACE";
        case OP_CLEAR: return "CLEAR";
        case OP_APPEND: return "APPEND";
        default: return "UNKNOWN";
    }
}

// Function to create an undo stack
UndoStack* createUndoStack(int maxOps) {
    UndoStack* stack = (UndoStack*)malloc(sizeof(UndoStack));
    stack->top = NULL;
    stack->operationCount = 0;
    stack->maxOperations = maxOps;
    return stack;
}

// Function to create a text editor
TextEditor* createTextEditor() {
    TextEditor* editor = (TextEditor*)malloc(sizeof(TextEditor));
    strcpy(editor->content, "");
    editor->undoStack = createUndoStack(MAX_OPERATIONS);
    editor->redoStack = createUndoStack(MAX_OPERATIONS);
    editor->nextOperationId = 1;
    editor->cursorPosition = 0;
    return editor;
}

// Function to check if stack is empty
int isStackEmpty(UndoStack* stack) {
    return stack->top == NULL;
}

// Function to push operation onto stack
void pushOperation(UndoStack* stack, OperationType type, int position, 
                  char* originalText, char* newText, char* deletedText) {
    
    // Remove oldest operation if stack is full
    if (stack->operationCount >= stack->maxOperations) {
        Operation* current = stack->top;
        Operation* prev = NULL;
        
        // Find the last operation
        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }
        
        if (prev != NULL) {
            prev->next = NULL;
        } else {
            stack->top = NULL;
        }
        
        free(current);
        stack->operationCount--;
    }
    
    Operation* newOp = (Operation*)malloc(sizeof(Operation));
    newOp->operationId = 0; // Will be set by caller
    newOp->type = type;
    newOp->position = position;
    strcpy(newOp->originalText, originalText ? originalText : "");
    strcpy(newOp->newText, newText ? newText : "");
    strcpy(newOp->deletedText, deletedText ? deletedText : "");
    newOp->timestamp = time(NULL);
    newOp->next = stack->top;
    
    stack->top = newOp;
    stack->operationCount++;
}

// Function to pop operation from stack
Operation* popOperation(UndoStack* stack) {
    if (isStackEmpty(stack)) {
        return NULL;
    }
    
    Operation* op = stack->top;
    stack->top = stack->top->next;
    stack->operationCount--;
    return op;
}

// Function to clear redo stack (called when new operation is performed)
void clearRedoStack(TextEditor* editor) {
    while (!isStackEmpty(editor->redoStack)) {
        Operation* op = popOperation(editor->redoStack);
        free(op);
    }
}

// Function to insert text at position
void insertText(TextEditor* editor, int position, char* text) {
    if (position < 0 || position > strlen(editor->content)) {
        printf("❌ Invalid position for insertion!\n");
        return;
    }
    
    if (strlen(editor->content) + strlen(text) >= MAX_TEXT_SIZE) {
        printf("❌ Text too long!\n");
        return;
    }
    
    char originalContent[MAX_TEXT_SIZE];
    strcpy(originalContent, editor->content);
    
    // Save current state for undo
    pushOperation(editor->undoStack, OP_INSERT, position, originalContent, text, "");
    editor->undoStack->top->operationId = editor->nextOperationId++;
    
    // Perform insertion
    char temp[MAX_TEXT_SIZE];
    strncpy(temp, editor->content, position);
    temp[position] = '\0';
    strcat(temp, text);
    strcat(temp, editor->content + position);
    strcpy(editor->content, temp);
    
    editor->cursorPosition = position + strlen(text);
    clearRedoStack(editor);
    
    printf("✅ Inserted \"%s\" at position %d\n", text, position);
}

// Function to delete text from position
void deleteText(TextEditor* editor, int position, int length) {
    if (position < 0 || position >= strlen(editor->content) || length <= 0) {
        printf("❌ Invalid position or length for deletion!\n");
        return;
    }
    
    int contentLen = strlen(editor->content);
    if (position + length > contentLen) {
        length = contentLen - position;
    }
    
    char originalContent[MAX_TEXT_SIZE];
    strcpy(originalContent, editor->content);
    
    char deletedText[MAX_TEXT_SIZE];
    strncpy(deletedText, editor->content + position, length);
    deletedText[length] = '\0';
    
    // Save current state for undo
    pushOperation(editor->undoStack, OP_DELETE, position, originalContent, "", deletedText);
    editor->undoStack->top->operationId = editor->nextOperationId++;
    
    // Perform deletion
    memmove(editor->content + position, editor->content + position + length, 
            strlen(editor->content + position + length) + 1);
    
    editor->cursorPosition = position;
    clearRedoStack(editor);
    
    printf("✅ Deleted \"%s\" from position %d\n", deletedText, position);
}

// Function to replace text
void replaceText(TextEditor* editor, int position, int length, char* newText) {
    if (position < 0 || position >= strlen(editor->content) || length <= 0) {
        printf("❌ Invalid position or length for replacement!\n");
        return;
    }
    
    int contentLen = strlen(editor->content);
    if (position + length > contentLen) {
        length = contentLen - position;
    }
    
    char originalContent[MAX_TEXT_SIZE];
    strcpy(originalContent, editor->content);
    
    char replacedText[MAX_TEXT_SIZE];
    strncpy(replacedText, editor->content + position, length);
    replacedText[length] = '\0';
    
    // Save current state for undo
    pushOperation(editor->undoStack, OP_REPLACE, position, originalContent, newText, replacedText);
    editor->undoStack->top->operationId = editor->nextOperationId++;
    
    // Perform replacement (delete then insert)
    memmove(editor->content + position, editor->content + position + length, 
            strlen(editor->content + position + length) + 1);
    
    char temp[MAX_TEXT_SIZE];
    strncpy(temp, editor->content, position);
    temp[position] = '\0';
    strcat(temp, newText);
    strcat(temp, editor->content + position);
    strcpy(editor->content, temp);
    
    editor->cursorPosition = position + strlen(newText);
    clearRedoStack(editor);
    
    printf("✅ Replaced \"%s\" with \"%s\" at position %d\n", 
           replacedText, newText, position);
}

// Function to append text
void appendText(TextEditor* editor, char* text) {
    insertText(editor, strlen(editor->content), text);
}

// Function to clear all text
void clearText(TextEditor* editor) {
    if (strlen(editor->content) == 0) {
        printf("❌ Text is already empty!\n");
        return;
    }
    
    char originalContent[MAX_TEXT_SIZE];
    strcpy(originalContent, editor->content);
    
    // Save current state for undo
    pushOperation(editor->undoStack, OP_CLEAR, 0, originalContent, "", originalContent);
    editor->undoStack->top->operationId = editor->nextOperationId++;
    
    strcpy(editor->content, "");
    editor->cursorPosition = 0;
    clearRedoStack(editor);
    
    printf("✅ Cleared all text\n");
}

// Function to undo last operation
void undoOperation(TextEditor* editor) {
    if (isStackEmpty(editor->undoStack)) {
        printf("❌ Nothing to undo!\n");
        return;
    }
    
    Operation* op = popOperation(editor->undoStack);
    
    printf("🔄 Undoing %s operation (ID: %d)\n", 
           getOperationTypeString(op->type), op->operationId);
    
    // Save current state to redo stack before undoing
    char currentContent[MAX_TEXT_SIZE];
    strcpy(currentContent, editor->content);
    
    // Push reverse operation to redo stack
    switch (op->type) {
        case OP_INSERT:
            pushOperation(editor->redoStack, OP_INSERT, op->position, 
                         currentContent, op->newText, "");
            break;
        case OP_DELETE:
            pushOperation(editor->redoStack, OP_DELETE, op->position, 
                         currentContent, "", op->deletedText);
            break;
        case OP_REPLACE:
            pushOperation(editor->redoStack, OP_REPLACE, op->position, 
                         currentContent, op->deletedText, op->newText);
            break;
        case OP_CLEAR:
            pushOperation(editor->redoStack, OP_CLEAR, 0, 
                         currentContent, "", op->deletedText);
            break;
        case OP_APPEND:
            pushOperation(editor->redoStack, OP_APPEND, op->position, 
                         currentContent, op->newText, "");
            break;
    }
    
    editor->redoStack->top->operationId = op->operationId;
    
    // Restore original state
    strcpy(editor->content, op->originalText);
    editor->cursorPosition = op->position;
    
    printf("✅ Undone successfully\n");
    free(op);
}

// Function to redo last undone operation
void redoOperation(TextEditor* editor) {
    if (isStackEmpty(editor->redoStack)) {
        printf("❌ Nothing to redo!\n");
        return;
    }
    
    Operation* op = popOperation(editor->redoStack);
    
    printf("🔄 Redoing %s operation (ID: %d)\n", 
           getOperationTypeString(op->type), op->operationId);
    
    // Re-execute the operation
    switch (op->type) {
        case OP_INSERT:
            // Don't use insertText to avoid creating new undo entry
            pushOperation(editor->undoStack, OP_INSERT, op->position, 
                         editor->content, op->newText, "");
            editor->undoStack->top->operationId = op->operationId;
            
            char temp[MAX_TEXT_SIZE];
            strncpy(temp, editor->content, op->position);
            temp[op->position] = '\0';
            strcat(temp, op->newText);
            strcat(temp, editor->content + op->position);
            strcpy(editor->content, temp);
            editor->cursorPosition = op->position + strlen(op->newText);
            break;
            
        case OP_DELETE:
            pushOperation(editor->undoStack, OP_DELETE, op->position, 
                         editor->content, "", op->deletedText);
            editor->undoStack->top->operationId = op->operationId;
            
            int delLen = strlen(op->deletedText);
            memmove(editor->content + op->position, 
                   editor->content + op->position + delLen, 
                   strlen(editor->content + op->position + delLen) + 1);
            editor->cursorPosition = op->position;
            break;
            
        case OP_CLEAR:
            pushOperation(editor->undoStack, OP_CLEAR, 0, 
                         editor->content, "", editor->content);
            editor->undoStack->top->operationId = op->operationId;
            
            strcpy(editor->content, "");
            editor->cursorPosition = 0;
            break;
            
        // Add other operation types as needed
    }
    
    printf("✅ Redone successfully\n");
    free(op);
}

// Function to display current text with cursor
void displayText(TextEditor* editor) {
    printf("\n=== CURRENT TEXT ===\n");
    
    if (strlen(editor->content) == 0) {
        printf("[Empty document]\n");
    } else {
        printf("Content: \"%s\"\n", editor->content);
    }
    
    printf("Length: %ld characters\n", strlen(editor->content));
    printf("Cursor position: %d\n", editor->cursorPosition);
    printf("===================\n\n");
}

// Function to display operation history
void displayOperationHistory(UndoStack* stack, const char* stackName) {
    if (isStackEmpty(stack)) {
        printf("%s stack is empty!\n", stackName);
        return;
    }
    
    printf("\n=== %s STACK ===\n", stackName);
    printf("%-4s %-10s %-8s %-20s %-15s\n", 
           "ID", "Type", "Pos", "Time", "Details");
    printf("----------------------------------------------------------\n");
    
    Operation* current = stack->top;
    int index = 1;
    
    while (current != NULL) {
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&current->timestamp));
        
        char details[50];
        switch (current->type) {
            case OP_INSERT:
                snprintf(details, sizeof(details), "Added: \"%.10s\"", current->newText);
                break;
            case OP_DELETE:
                snprintf(details, sizeof(details), "Deleted: \"%.10s\"", current->deletedText);
                break;
            case OP_REPLACE:
                snprintf(details, sizeof(details), "\"%.8s\"->\"%.8s\"", 
                        current->deletedText, current->newText);
                break;
            case OP_CLEAR:
                snprintf(details, sizeof(details), "Cleared all");
                break;
            case OP_APPEND:
                snprintf(details, sizeof(details), "Appended: \"%.10s\"", current->newText);
                break;
        }
        
        printf("%-4d %-10s %-8d %-20s %-15s", 
               current->operationId,
               getOperationTypeString(current->type),
               current->position,
               timeStr,
               details);
        
        if (index == 1) {
            printf(" <- TOP");
        }
        printf("\n");
        
        current = current->next;
        index++;
    }
    printf("Total operations: %d\n", stack->operationCount);
    printf("========================================\n\n");
}

// Function to demonstrate undo/redo functionality
void demonstrateUndoRedo(TextEditor* editor) {
    printf("\n=== UNDO/REDO DEMONSTRATION ===\n");
    
    // Clear any existing content
    if (strlen(editor->content) > 0) {
        clearText(editor);
    }
    
    // Perform a series of operations
    printf("Performing a series of operations...\n\n");
    
    insertText(editor, 0, "Hello");
    displayText(editor);
    
    insertText(editor, 5, " World");
    displayText(editor);
    
    insertText(editor, 11, "!");
    displayText(editor);
    
    replaceText(editor, 6, 5, "Universe");
    displayText(editor);
    
    printf("Operation history:\n");
    displayOperationHistory(editor->undoStack, "UNDO");
    
    // Demonstrate undo operations
    printf("Now demonstrating undo operations...\n\n");
    
    undoOperation(editor); // Undo replace
    displayText(editor);
    
    undoOperation(editor); // Undo insert "!"
    displayText(editor);
    
    undoOperation(editor); // Undo insert " World"
    displayText(editor);
    
    printf("After undos - Redo stack:\n");
    displayOperationHistory(editor->redoStack, "REDO");
    
    // Demonstrate redo operations
    printf("Now demonstrating redo operations...\n\n");
    
    redoOperation(editor); // Redo insert " World"
    displayText(editor);
    
    redoOperation(editor); // Redo insert "!"
    displayText(editor);
    
    printf("Demonstration completed!\n");
}

// Function to free editor memory
void freeTextEditor(TextEditor* editor) {
    // Free undo stack
    while (!isStackEmpty(editor->undoStack)) {
        Operation* op = popOperation(editor->undoStack);
        free(op);
    }
    free(editor->undoStack);
    
    // Free redo stack
    while (!isStackEmpty(editor->redoStack)) {
        Operation* op = popOperation(editor->redoStack);
        free(op);
    }
    free(editor->redoStack);
    
    free(editor);
}

// Main function with menu-driven interface
int main() {
    TextEditor* editor = createTextEditor();
    int choice, position, length;
    char text[MAX_TEXT_SIZE];
    
    printf("↩️  Welcome to Undo Operation System using Stack! ↩️\n");
    printf("Experience powerful undo/redo functionality!\n\n");
    
    do {
        printf("\n=== TEXT EDITOR MENU ===\n");
        printf("1. Display current text\n");
        printf("2. Insert text at position\n");
        printf("3. Delete text from position\n");
        printf("4. Replace text\n");
        printf("5. Append text\n");
        printf("6. Clear all text\n");
        printf("7. Undo operation\n");
        printf("8. Redo operation\n");
        printf("9. Show undo history\n");
        printf("10. Show redo history\n");
        printf("11. Demonstrate undo/redo\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                displayText(editor);
                break;
                
            case 2:
                printf("Enter position to insert at: ");
                scanf("%d", &position);
                getchar();
                
                printf("Enter text to insert: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = 0;
                
                insertText(editor, position, text);
                break;
                
            case 3:
                printf("Enter position to delete from: ");
                scanf("%d", &position);
                
                printf("Enter length to delete: ");
                scanf("%d", &length);
                
                deleteText(editor, position, length);
                break;
                
            case 4:
                printf("Enter position to replace from: ");
                scanf("%d", &position);
                
                printf("Enter length to replace: ");
                scanf("%d", &length);
                getchar();
                
                printf("Enter replacement text: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = 0;
                
                replaceText(editor, position, length, text);
                break;
                
            case 5:
                printf("Enter text to append: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = 0;
                
                appendText(editor, text);
                break;
                
            case 6:
                clearText(editor);
                break;
                
            case 7:
                undoOperation(editor);
                break;
                
            case 8:
                redoOperation(editor);
                break;
                
            case 9:
                displayOperationHistory(editor->undoStack, "UNDO");
                break;
                
            case 10:
                displayOperationHistory(editor->redoStack, "REDO");
                break;
                
            case 11:
                demonstrateUndoRedo(editor);
                break;
                
            case 12:
                printf("Thanks for using the Undo Operation System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 12);
    
    freeTextEditor(editor);
    return 0;
}