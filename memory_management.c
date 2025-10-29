/*
 * Memory Management Model using Stack
 * This program simulates memory allocation and deallocation using stack
 * to demonstrate stack-based memory management concepts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STACK_SIZE 1000
#define MAX_BLOCK_SIZE 100

// Enumeration for memory block types
typedef enum {
    BLOCK_INT,
    BLOCK_FLOAT,
    BLOCK_CHAR,
    BLOCK_STRING,
    BLOCK_ARRAY
} BlockType;

// Structure to represent a memory block
typedef struct MemoryBlock {
    int blockId;
    BlockType type;
    char variableName[50];
    void* data;
    size_t size;
    time_t allocTime;
    struct MemoryBlock* next;
} MemoryBlock;

// Structure to represent the memory stack
typedef struct MemoryStack {
    MemoryBlock* top;
    int totalBlocks;
    size_t totalMemoryUsed;
    size_t maxMemoryUsed;
    int nextBlockId;
} MemoryStack;

// Function to get block type string
const char* getBlockTypeString(BlockType type) {
    switch (type) {
        case BLOCK_INT: return "int";
        case BLOCK_FLOAT: return "float";
        case BLOCK_CHAR: return "char";
        case BLOCK_STRING: return "string";
        case BLOCK_ARRAY: return "array";
        default: return "unknown";
    }
}

// Function to create a memory stack
MemoryStack* createMemoryStack() {
    MemoryStack* stack = (MemoryStack*)malloc(sizeof(MemoryStack));
    stack->top = NULL;
    stack->totalBlocks = 0;
    stack->totalMemoryUsed = 0;
    stack->maxMemoryUsed = 0;
    stack->nextBlockId = 1001;
    return stack;
}

// Function to check if stack is empty
int isMemoryStackEmpty(MemoryStack* stack) {
    return stack->top == NULL;
}

// Function to allocate memory block on stack
int allocateMemory(MemoryStack* stack, BlockType type, char* varName, size_t size) {
    if (stack->totalMemoryUsed + size > MAX_STACK_SIZE) {
        printf("Error: Stack overflow! Cannot allocate %zu bytes\n", size);
        return 0;
    }
    
    MemoryBlock* newBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    newBlock->blockId = stack->nextBlockId++;
    newBlock->type = type;
    strcpy(newBlock->variableName, varName);
    newBlock->size = size;
    newBlock->allocTime = time(NULL);
    newBlock->next = stack->top;
    
    // Allocate actual data space
    newBlock->data = malloc(size);
    if (newBlock->data == NULL) {
        printf("Error: Failed to allocate memory for data\n");
        free(newBlock);
        return 0;
    }
    
    // Initialize data based on type
    switch (type) {
        case BLOCK_INT:
            *(int*)newBlock->data = 0;
            break;
        case BLOCK_FLOAT:
            *(float*)newBlock->data = 0.0f;
            break;
        case BLOCK_CHAR:
            *(char*)newBlock->data = '\0';
            break;
        case BLOCK_STRING:
            strcpy((char*)newBlock->data, "");
            break;
        case BLOCK_ARRAY:
            memset(newBlock->data, 0, size);
            break;
    }
    
    stack->top = newBlock;
    stack->totalBlocks++;
    stack->totalMemoryUsed += size;
    
    if (stack->totalMemoryUsed > stack->maxMemoryUsed) {
        stack->maxMemoryUsed = stack->totalMemoryUsed;
    }
    
    printf("Allocated %zu bytes for %s variable '%s' (Block ID: %d)\n", 
        size, getBlockTypeString(type), varName, newBlock->blockId);
    
    return newBlock->blockId;
}

// Function to deallocate top memory block
int deallocateMemory(MemoryStack* stack) {
    if (isMemoryStackEmpty(stack)) {
        printf("Error: Stack underflow! No memory to deallocate\n");
        return 0;
    }
    
    MemoryBlock* topBlock = stack->top;
    stack->top = stack->top->next;
    
    printf("Deallocated %zu bytes from %s variable '%s' (Block ID: %d)\n",
        topBlock->size, getBlockTypeString(topBlock->type), 
        topBlock->variableName, topBlock->blockId);
    
    stack->totalBlocks--;
    stack->totalMemoryUsed -= topBlock->size;
    
    free(topBlock->data);
    free(topBlock);
    
    return 1;
}

// Function to set value in top memory block
void setTopBlockValue(MemoryStack* stack, void* value) {
    if (isMemoryStackEmpty(stack)) {
        printf("Error: No memory block to set value\n");
        return;
    }
    
    MemoryBlock* topBlock = stack->top;
    
    switch (topBlock->type) {
        case BLOCK_INT:
            *(int*)topBlock->data = *(int*)value;
            printf("Set %s '%s' = %d\n", getBlockTypeString(topBlock->type), 
                   topBlock->variableName, *(int*)value);
            break;
            
        case BLOCK_FLOAT:
            *(float*)topBlock->data = *(float*)value;
            printf("Set %s '%s' = %.2f\n", getBlockTypeString(topBlock->type), 
                   topBlock->variableName, *(float*)value);
            break;
            
        case BLOCK_CHAR:
            *(char*)topBlock->data = *(char*)value;
            printf("Set %s '%s' = '%c'\n", getBlockTypeString(topBlock->type), 
                   topBlock->variableName, *(char*)value);
            break;
            
        case BLOCK_STRING:
            strcpy((char*)topBlock->data, (char*)value);
            printf("Set %s '%s' = \"%s\"\n", getBlockTypeString(topBlock->type), 
                   topBlock->variableName, (char*)value);
            break;
            
        case BLOCK_ARRAY:
            memcpy(topBlock->data, value, topBlock->size);
            printf("Set %s '%s' with array data\n", getBlockTypeString(topBlock->type), 
                   topBlock->variableName);
            break;
    }
}

// Function to display top memory block value
void displayTopBlockValue(MemoryStack* stack) {
    if (isMemoryStackEmpty(stack)) {
        printf("Error: No memory block to display\n");
        return;
    }
    
    MemoryBlock* topBlock = stack->top;
    printf("Top block value - %s '%s': ", 
           getBlockTypeString(topBlock->type), topBlock->variableName);
    
    switch (topBlock->type) {
        case BLOCK_INT:
            printf("%d\n", *(int*)topBlock->data);
            break;
            
        case BLOCK_FLOAT:
            printf("%.2f\n", *(float*)topBlock->data);
            break;
            
        case BLOCK_CHAR:
            printf("'%c'\n", *(char*)topBlock->data);
            break;
            
        case BLOCK_STRING:
            printf("\"%s\"\n", (char*)topBlock->data);
            break;
            
        case BLOCK_ARRAY:
            printf("[array data %zu bytes]\n", topBlock->size);
            break;
    }
}

// Function to display entire memory stack
void displayMemoryStack(MemoryStack* stack) {
    if (isMemoryStackEmpty(stack)) {
        printf("Memory stack is empty!\n");
        return;
    }
    
    printf("\n=== MEMORY STACK (Top to Bottom) ===\n");
    printf("%-6s %-10s %-15s %-8s %-20s\n", 
           "ID", "Type", "Variable", "Size", "Allocation Time");
    printf("--------------------------------------------------------\n");
    
    MemoryBlock* current = stack->top;
    int position = 1;
    
    while (current != NULL) {
        char timeStr[50];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&current->allocTime));
        
        printf("%-6d %-10s %-15s %-8zu %-20s", 
               current->blockId, 
               getBlockTypeString(current->type),
               current->variableName,
               current->size,
               timeStr);
        
        if (position == 1) {
            printf(" <- TOP");
        }
        printf("\n");
        
        current = current->next;
        position++;
    }
    printf("========================================\n\n");
}

// Function to display memory statistics
void displayMemoryStats(MemoryStack* stack) {
    printf("\n=== MEMORY STATISTICS ===\n");
    printf("Total blocks: %d\n", stack->totalBlocks);
    printf("Memory used: %zu bytes\n", stack->totalMemoryUsed);
    printf("Max memory used: %zu bytes\n", stack->maxMemoryUsed);
    printf("Available memory: %d bytes\n", MAX_STACK_SIZE - (int)stack->totalMemoryUsed);
    printf("Memory utilization: %.1f%%\n", 
           (float)stack->totalMemoryUsed / MAX_STACK_SIZE * 100);
    printf("Next block ID: %d\n", stack->nextBlockId);
    printf("=========================\n\n");
}

// Function to simulate function call with local variables
void simulateFunctionCall(MemoryStack* stack, char* functionName) {
    printf("\n=== SIMULATING FUNCTION CALL: %s ===\n", functionName);
    
    // Create stack frame marker
    printf("Creating stack frame for function %s...\n", functionName);
    
    // Allocate local variables
    allocateMemory(stack, BLOCK_INT, "localVar1", sizeof(int));
    int val1 = 42;
    setTopBlockValue(stack, &val1);
    
    allocateMemory(stack, BLOCK_FLOAT, "localVar2", sizeof(float));
    float val2 = 3.14f;
    setTopBlockValue(stack, &val2);
    
    allocateMemory(stack, BLOCK_STRING, "message", 50);
    char msg[] = "Hello from function";
    setTopBlockValue(stack, msg);
    
    printf("Function %s is executing with local variables:\n", functionName);
    displayMemoryStack(stack);
    
    printf("Function %s completed. Cleaning up local variables...\n", functionName);
    
    // Clean up local variables (simulate function return)
    deallocateMemory(stack); // message
    deallocateMemory(stack); // localVar2
    deallocateMemory(stack); // localVar1
    
    printf("Stack frame for %s cleaned up.\n", functionName);
}

// Function to simulate nested function calls
void simulateNestedCalls(MemoryStack* stack) {
    printf("\n=== SIMULATING NESTED FUNCTION CALLS ===\n");
    
    // Main function variables
    allocateMemory(stack, BLOCK_INT, "mainVar", sizeof(int));
    int mainVal = 100;
    setTopBlockValue(stack, &mainVal);
    
    printf("Main function started.\n");
    displayMemoryStack(stack);
    
    // First function call
    printf("\nCalling function A...\n");
    allocateMemory(stack, BLOCK_FLOAT, "functionA_var", sizeof(float));
    float aVal = 1.5f;
    setTopBlockValue(stack, &aVal);
    
    // Second function call (nested)
    printf("\nCalling function B from A...\n");
    allocateMemory(stack, BLOCK_CHAR, "functionB_var", sizeof(char));
    char bVal = 'X';
    setTopBlockValue(stack, &bVal);
    
    printf("\nCurrent stack state (nested calls):\n");
    displayMemoryStack(stack);
    
    // Return from function B
    printf("\nReturning from function B...\n");
    deallocateMemory(stack);
    
    // Return from function A
    printf("\nReturning from function A...\n");
    deallocateMemory(stack);
    
    printf("\nBack in main function:\n");
    displayMemoryStack(stack);
    
    // Clean up main
    deallocateMemory(stack);
    printf("\nMain function completed.\n");
}

// Function to demonstrate memory leak detection
void demonstrateMemoryLeak(MemoryStack* stack) {
    printf("\n=== MEMORY LEAK DEMONSTRATION ===\n");
    
    printf("Allocating memory without proper cleanup...\n");
    
    allocateMemory(stack, BLOCK_ARRAY, "leakyArray1", 50);
    allocateMemory(stack, BLOCK_STRING, "leakyString", 30);
    allocateMemory(stack, BLOCK_INT, "leakyInt", sizeof(int));
    
    printf("Current memory usage:\n");
    displayMemoryStats(stack);
    
    printf("Simulating program exit without cleanup...\n");
    printf("Memory leak detected: %zu bytes not freed!\n", stack->totalMemoryUsed);
    
    printf("Proper cleanup:\n");
    while (!isMemoryStackEmpty(stack)) {
        deallocateMemory(stack);
    }
    
    printf("All memory cleaned up.\n");
}

// Function to free entire memory stack
void freeMemoryStack(MemoryStack* stack) {
    while (!isMemoryStackEmpty(stack)) {
        deallocateMemory(stack);
    }
    free(stack);
}

// Main function with menu-driven interface
int main() {
    MemoryStack* memStack = createMemoryStack();
    int choice, intVal, blockType;
    float floatVal;
    char charVal, varName[50], stringVal[100];
    size_t arraySize;
    
    printf("Welcome to Memory Management Model using Stack!\n");
    printf("Simulate stack-based memory allocation and deallocation!\n\n");
    
    do {
        printf("\n=== MEMORY MANAGEMENT MENU ===\n");
        printf("1. Allocate memory\n");
        printf("2. Deallocate memory (top block)\n");
        printf("3. Set value in top block\n");
        printf("4. Display top block value\n");
        printf("5. Display memory stack\n");
        printf("6. Display memory statistics\n");
        printf("7. Simulate function call\n");
        printf("8. Simulate nested function calls\n");
        printf("9. Demonstrate memory leak\n");
        printf("10. Clear all memory\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Block types: 0-int, 1-float, 2-char, 3-string, 4-array\n");
                printf("Enter block type (0-4): ");
                scanf("%d", &blockType);
                getchar();
                
                printf("Enter variable name: ");
                fgets(varName, sizeof(varName), stdin);
                varName[strcspn(varName, "\n")] = 0;
                
                switch (blockType) {
                    case BLOCK_INT:
                        allocateMemory(memStack, BLOCK_INT, varName, sizeof(int));
                        break;
                    case BLOCK_FLOAT:
                        allocateMemory(memStack, BLOCK_FLOAT, varName, sizeof(float));
                        break;
                    case BLOCK_CHAR:
                        allocateMemory(memStack, BLOCK_CHAR, varName, sizeof(char));
                        break;
                    case BLOCK_STRING:
                        printf("Enter string size: ");
                        scanf("%zu", &arraySize);
                        if (arraySize > 0 && arraySize <= MAX_BLOCK_SIZE) {
                            allocateMemory(memStack, BLOCK_STRING, varName, arraySize);
                        } else {
                            printf("Invalid size!\n");
                        }
                        break;
                    case BLOCK_ARRAY:
                        printf("Enter array size in bytes: ");
                        scanf("%zu", &arraySize);
                        if (arraySize > 0 && arraySize <= MAX_BLOCK_SIZE) {
                            allocateMemory(memStack, BLOCK_ARRAY, varName, arraySize);
                        } else {
                            printf("Invalid size!\n");
                        }
                        break;
                    default:
                        printf("Invalid block type!\n");
                }
                break;
                
            case 2:
                deallocateMemory(memStack);
                break;
                
            case 3:
                if (!isMemoryStackEmpty(memStack)) {
                    MemoryBlock* top = memStack->top;
                    switch (top->type) {
                        case BLOCK_INT:
                            printf("Enter integer value: ");
                            scanf("%d", &intVal);
                            setTopBlockValue(memStack, &intVal);
                            break;
                        case BLOCK_FLOAT:
                            printf("Enter float value: ");
                            scanf("%f", &floatVal);
                            setTopBlockValue(memStack, &floatVal);
                            break;
                        case BLOCK_CHAR:
                            printf("Enter character value: ");
                            scanf(" %c", &charVal);
                            setTopBlockValue(memStack, &charVal);
                            break;
                        case BLOCK_STRING:
                            getchar();
                            printf("Enter string value: ");
                            fgets(stringVal, sizeof(stringVal), stdin);
                            stringVal[strcspn(stringVal, "\n")] = 0;
                            setTopBlockValue(memStack, stringVal);
                            break;
                        case BLOCK_ARRAY:
                            printf("Array value setting not implemented in this demo\n");
                            break;
                    }
                } else {
                    printf("No memory block to set value!\n");
                }
                break;
                
            case 4:
                displayTopBlockValue(memStack);
                break;
                
            case 5:
                displayMemoryStack(memStack);
                break;
                
            case 6:
                displayMemoryStats(memStack);
                break;
                
            case 7:
                simulateFunctionCall(memStack, "testFunction");
                break;
                
            case 8:
                simulateNestedCalls(memStack);
                break;
                
            case 9:
                demonstrateMemoryLeak(memStack);
                break;
                
            case 10:
                while (!isMemoryStackEmpty(memStack)) {
                    deallocateMemory(memStack);
                }
                printf("All memory cleared!\n");
                break;
                
            case 11:
                printf("Thanks for using Memory Management Model!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 11);
    
    freeMemoryStack(memStack);
    return 0;
}