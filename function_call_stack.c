/*
 * Function Call Stack Implementation
 * This program demonstrates how stack is used to store and restore data
 * in function calls, including parameter passing and local variable management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FUNCTION_NAME 50
#define MAX_VARIABLE_NAME 30
#define MAX_STACK_FRAMES 100

// Enumeration for variable types
typedef enum {
    VAR_INT,
    VAR_FLOAT,
    VAR_CHAR,
    VAR_STRING
} VariableType;

// Structure to represent a variable in a stack frame
typedef struct Variable {
    char name[MAX_VARIABLE_NAME];
    VariableType type;
    union {
        int intVal;
        float floatVal;
        char charVal;
        char stringVal[100];
    } value;
    struct Variable* next;
} Variable;

// Structure to represent a stack frame (function call)
typedef struct StackFrame {
    char functionName[MAX_FUNCTION_NAME];
    int frameId;
    Variable* parameters;
    Variable* localVariables;
    int returnAddress; // Simulated return address
    time_t callTime;
    struct StackFrame* next;
} StackFrame;

// Structure to represent the function call stack
typedef struct FunctionCallStack {
    StackFrame* top;
    int frameCount;
    int nextFrameId;
    int currentExecutionLevel;
} FunctionCallStack;

// Function to get variable type string
const char* getVariableTypeString(VariableType type) {
    switch (type) {
        case VAR_INT: return "int";
        case VAR_FLOAT: return "float";
        case VAR_CHAR: return "char";
        case VAR_STRING: return "string";
        default: return "unknown";
    }
}

// Function to create a function call stack
FunctionCallStack* createFunctionCallStack() {
    FunctionCallStack* stack = (FunctionCallStack*)malloc(sizeof(FunctionCallStack));
    stack->top = NULL;
    stack->frameCount = 0;
    stack->nextFrameId = 1;
    stack->currentExecutionLevel = 0;
    return stack;
}

// Function to create a new variable
Variable* createVariable(char* name, VariableType type) {
    Variable* var = (Variable*)malloc(sizeof(Variable));
    strcpy(var->name, name);
    var->type = type;
    var->next = NULL;
    
    // Initialize value based on type
    switch (type) {
        case VAR_INT:
            var->value.intVal = 0;
            break;
        case VAR_FLOAT:
            var->value.floatVal = 0.0f;
            break;
        case VAR_CHAR:
            var->value.charVal = '\0';
            break;
        case VAR_STRING:
            strcpy(var->value.stringVal, "");
            break;
    }
    
    return var;
}

// Function to add variable to list
void addVariable(Variable** varList, char* name, VariableType type) {
    Variable* newVar = createVariable(name, type);
    newVar->next = *varList;
    *varList = newVar;
}

// Function to set variable value
void setVariableValue(Variable* var, void* value) {
    switch (var->type) {
        case VAR_INT:
            var->value.intVal = *(int*)value;
            break;
        case VAR_FLOAT:
            var->value.floatVal = *(float*)value;
            break;
        case VAR_CHAR:
            var->value.charVal = *(char*)value;
            break;
        case VAR_STRING:
            strcpy(var->value.stringVal, (char*)value);
            break;
    }
}

// Function to find variable in list
Variable* findVariable(Variable* varList, char* name) {
    Variable* current = varList;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to display variable value
void displayVariableValue(Variable* var) {
    printf("%s %s = ", getVariableTypeString(var->type), var->name);
    
    switch (var->type) {
        case VAR_INT:
            printf("%d", var->value.intVal);
            break;
        case VAR_FLOAT:
            printf("%.2f", var->value.floatVal);
            break;
        case VAR_CHAR:
            printf("'%c'", var->value.charVal);
            break;
        case VAR_STRING:
            printf("\"%s\"", var->value.stringVal);
            break;
    }
    printf("\n");
}

// Function to display variable list
void displayVariableList(Variable* varList, const char* listName) {
    if (varList == NULL) {
        printf("  %s: None\n", listName);
        return;
    }
    
    printf("  %s:\n", listName);
    Variable* current = varList;
    while (current != NULL) {
        printf("    ");
        displayVariableValue(current);
        current = current->next;
    }
}

// Function to check if stack is empty
int isFunctionStackEmpty(FunctionCallStack* stack) {
    return stack->top == NULL;
}

// Function to push a new function call onto the stack
StackFrame* pushFunctionCall(FunctionCallStack* stack, char* functionName, int returnAddr) {
    if (stack->frameCount >= MAX_STACK_FRAMES) {
        printf("Error: Stack overflow! Maximum function call depth reached.\n");
        return NULL;
    }
    
    StackFrame* newFrame = (StackFrame*)malloc(sizeof(StackFrame));
    strcpy(newFrame->functionName, functionName);
    newFrame->frameId = stack->nextFrameId++;
    newFrame->parameters = NULL;
    newFrame->localVariables = NULL;
    newFrame->returnAddress = returnAddr;
    newFrame->callTime = time(NULL);
    newFrame->next = stack->top;
    
    stack->top = newFrame;
    stack->frameCount++;
    stack->currentExecutionLevel++;
    
    printf("Function call: %s() [Frame ID: %d, Level: %d]\n", 
        functionName, newFrame->frameId, stack->currentExecutionLevel);
    
    return newFrame;
}

// Function to pop function call from stack (function return)
StackFrame* popFunctionCall(FunctionCallStack* stack) {
    if (isFunctionStackEmpty(stack)) {
        printf("Error: Stack underflow! No function to return from.\n");
        return NULL;
    }
    
    StackFrame* frame = stack->top;
    stack->top = stack->top->next;
    stack->frameCount--;
    stack->currentExecutionLevel--;
    
    printf("Function return: %s() [Frame ID: %d, Return to level: %d]\n", 
        frame->functionName, frame->frameId, stack->currentExecutionLevel);
    
    return frame;
}

// Function to add parameter to current function
void addParameter(FunctionCallStack* stack, char* name, VariableType type, void* value) {
    if (isFunctionStackEmpty(stack)) {
        printf("Error: No active function call to add parameter to!\n");
        return;
    }
    
    StackFrame* currentFrame = stack->top;
    addVariable(&currentFrame->parameters, name, type);
    
    Variable* param = findVariable(currentFrame->parameters, name);
    if (param != NULL) {
        setVariableValue(param, value);
        printf("Parameter added: ");
        displayVariableValue(param);
    }
}

// Function to add local variable to current function
void addLocalVariable(FunctionCallStack* stack, char* name, VariableType type) {
    if (isFunctionStackEmpty(stack)) {
        printf("Error: No active function call to add local variable to!\n");
        return;
    }
    
    StackFrame* currentFrame = stack->top;
    addVariable(&currentFrame->localVariables, name, type);
    
    printf("Local variable declared: %s %s\n", getVariableTypeString(type), name);
}

// Function to set local variable value
void setLocalVariableValue(FunctionCallStack* stack, char* name, void* value) {
    if (isFunctionStackEmpty(stack)) {
        printf("Error: No active function call!\n");
        return;
    }
    
    StackFrame* currentFrame = stack->top;
    Variable* var = findVariable(currentFrame->localVariables, name);
    
    if (var != NULL) {
        setVariableValue(var, value);
        printf("Variable updated: ");
        displayVariableValue(var);
    } else {
        printf("Error: Local variable '%s' not found!\n", name);
    }
}

// Function to display current stack frame
void displayCurrentFrame(FunctionCallStack* stack) {
    if (isFunctionStackEmpty(stack)) {
        printf("No active function call!\n");
        return;
    }
    
    StackFrame* frame = stack->top;
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&frame->callTime));
    
    printf("\n=== CURRENT STACK FRAME ===\n");
    printf("Function: %s()\n", frame->functionName);
    printf("Frame ID: %d\n", frame->frameId);
    printf("Execution Level: %d\n", stack->currentExecutionLevel);
    printf("Return Address: %d\n", frame->returnAddress);
    printf("Call Time: %s\n", timeStr);
    
    displayVariableList(frame->parameters, "Parameters");
    displayVariableList(frame->localVariables, "Local Variables");
    printf("===========================\n\n");
}

// Function to display entire call stack
void displayCallStack(FunctionCallStack* stack) {
    if (isFunctionStackEmpty(stack)) {
        printf("Function call stack is empty!\n");
        return;
    }
    
    printf("\n=== FUNCTION CALL STACK ===\n");
    printf("Stack depth: %d\n", stack->frameCount);
    printf("Current execution level: %d\n\n", stack->currentExecutionLevel);
    
    StackFrame* current = stack->top;
    int level = stack->currentExecutionLevel;
    
    while (current != NULL) {
        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&current->callTime));
        
        printf("Level %d: %s() [ID: %d, Called: %s]\n", 
               level, current->functionName, current->frameId, timeStr);
        
        // Show parameter count and local variable count
        int paramCount = 0, localCount = 0;
        Variable* var = current->parameters;
        while (var) { paramCount++; var = var->next; }
        
        var = current->localVariables;
        while (var) { localCount++; var = var->next; }
        
        printf("         Parameters: %d, Local vars: %d\n", paramCount, localCount);
        
        current = current->next;
        level--;
    }
    printf("===========================\n\n");
}

// Function to simulate a recursive function (factorial)
void simulateFactorial(FunctionCallStack* stack, int n) {
    printf("\n=== SIMULATING FACTORIAL(%d) ===\n", n);
    
    // Base case
    if (n <= 1) {
        pushFunctionCall(stack, "factorial", 100 + n);
        addParameter(stack, "n", VAR_INT, &n);
        
        int result = 1;
        addLocalVariable(stack, "result", VAR_INT);
        setLocalVariableValue(stack, "result", &result);
        
        printf("Base case reached: factorial(%d) = 1\n", n);
        displayCurrentFrame(stack);
        
        popFunctionCall(stack);
        return;
    }
    
    // Recursive case
    pushFunctionCall(stack, "factorial", 100 + n);
    addParameter(stack, "n", VAR_INT, &n);
    
    addLocalVariable(stack, "result", VAR_INT);
    
    printf("Recursive call: factorial(%d) needs factorial(%d)\n", n, n-1);
    displayCallStack(stack);
    
    // Recursive call
    simulateFactorial(stack, n - 1);
    
    // Calculate result after recursive call returns
    int result = n; // In real implementation, this would be n * factorial(n-1)
    setLocalVariableValue(stack, "result", &result);
    
    printf("Computed factorial(%d) (simplified)\n", n);
    displayCurrentFrame(stack);
    
    popFunctionCall(stack);
}

// Function to simulate nested function calls
void simulateNestedFunctions(FunctionCallStack* stack) {
    printf("\n=== SIMULATING NESTED FUNCTION CALLS ===\n");
    
    // Main function
    pushFunctionCall(stack, "main", 1000);
    int mainVar = 42;
    addLocalVariable(stack, "x", VAR_INT);
    setLocalVariableValue(stack, "x", &mainVar);
    
    char message[] = "Hello";
    addLocalVariable(stack, "msg", VAR_STRING);
    setLocalVariableValue(stack, "msg", message);
    
    displayCurrentFrame(stack);
    
    // Call function A
    pushFunctionCall(stack, "functionA", 2000);
    float aParam = 3.14f;
    addParameter(stack, "pi", VAR_FLOAT, &aParam);
    
    float radius = 5.0f;
    addLocalVariable(stack, "radius", VAR_FLOAT);
    setLocalVariableValue(stack, "radius", &radius);
    
    displayCallStack(stack);
    
    // Call function B from A
    pushFunctionCall(stack, "functionB", 3000);
    char bParam = 'B';
    addParameter(stack, "grade", VAR_CHAR, &bParam);
    
    int score = 85;
    addLocalVariable(stack, "score", VAR_INT);
    setLocalVariableValue(stack, "score", &score);
    
    displayCallStack(stack);
    
    // Return from function B
    popFunctionCall(stack);
    
    // Return from function A
    popFunctionCall(stack);
    
    // Return from main
    popFunctionCall(stack);
    
    printf("All functions returned. Stack is now empty.\n");
}

// Function to demonstrate stack overflow
void simulateStackOverflow(FunctionCallStack* stack, int depth) {
    printf("\n=== SIMULATING STACK OVERFLOW ===\n");
    printf("Attempting to create %d nested function calls...\n", depth);
    
    for (int i = 0; i < depth; i++) {
        char funcName[50];
        sprintf(funcName, "function_%d", i);
        
        StackFrame* frame = pushFunctionCall(stack, funcName, 4000 + i);
        if (frame == NULL) {
            printf("Stack overflow occurred at depth %d\n", i);
            break;
        }
        
        // Add some variables to each frame
        addLocalVariable(stack, "localVar", VAR_INT);
        int val = i;
        setLocalVariableValue(stack, "localVar", &val);
        
        if (i % 10 == 0) {
            printf("Current stack depth: %d\n", stack->frameCount);
        }
    }
    
    printf("Cleaning up function calls...\n");
    while (!isFunctionStackEmpty(stack)) {
        popFunctionCall(stack);
    }
}

// Function to free variable list
void freeVariableList(Variable* varList) {
    while (varList != NULL) {
        Variable* next = varList->next;
        free(varList);
        varList = next;
    }
}

// Function to free function call stack
void freeFunctionCallStack(FunctionCallStack* stack) {
    while (!isFunctionStackEmpty(stack)) {
        StackFrame* frame = popFunctionCall(stack);
        freeVariableList(frame->parameters);
        freeVariableList(frame->localVariables);
        free(frame);
    }
    free(stack);
}

// Main function with menu-driven interface
int main() {
    FunctionCallStack* callStack = createFunctionCallStack();
    int choice, intVal, returnAddr;
    float floatVal;
    char charVal, funcName[MAX_FUNCTION_NAME], varName[MAX_VARIABLE_NAME], stringVal[100];
    int varType;
    
    printf("Welcome to Function Call Stack Implementation!\n");
    printf("Explore how function calls use stack for data management!\n\n");
    
    do {
        printf("\n=== FUNCTION CALL STACK MENU ===\n");
        printf("1. Push function call\n");
        printf("2. Pop function call (return)\n");
        printf("3. Add parameter to current function\n");
        printf("4. Add local variable\n");
        printf("5. Set local variable value\n");
        printf("6. Display current frame\n");
        printf("7. Display entire call stack\n");
        printf("8. Simulate factorial recursion\n");
        printf("9. Simulate nested function calls\n");
        printf("10. Simulate stack overflow\n");
        printf("11. Clear all function calls\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter function name: ");
                fgets(funcName, sizeof(funcName), stdin);
                funcName[strcspn(funcName, "\n")] = 0;
                
                printf("Enter return address: ");
                scanf("%d", &returnAddr);
                
                pushFunctionCall(callStack, funcName, returnAddr);
                break;
                
            case 2:
                {
                    StackFrame* frame = popFunctionCall(callStack);
                    if (frame != NULL) {
                        freeVariableList(frame->parameters);
                        freeVariableList(frame->localVariables);
                        free(frame);
                    }
                }
                break;
                
            case 3:
                printf("Variable types: 0-int, 1-float, 2-char, 3-string\n");
                printf("Enter parameter type (0-3): ");
                scanf("%d", &varType);
                getchar();
                
                printf("Enter parameter name: ");
                fgets(varName, sizeof(varName), stdin);
                varName[strcspn(varName, "\n")] = 0;
                
                switch (varType) {
                    case VAR_INT:
                        printf("Enter integer value: ");
                        scanf("%d", &intVal);
                        addParameter(callStack, varName, VAR_INT, &intVal);
                        break;
                    case VAR_FLOAT:
                        printf("Enter float value: ");
                        scanf("%f", &floatVal);
                        addParameter(callStack, varName, VAR_FLOAT, &floatVal);
                        break;
                    case VAR_CHAR:
                        printf("Enter char value: ");
                        scanf(" %c", &charVal);
                        addParameter(callStack, varName, VAR_CHAR, &charVal);
                        break;
                    case VAR_STRING:
                        getchar();
                        printf("Enter string value: ");
                        fgets(stringVal, sizeof(stringVal), stdin);
                        stringVal[strcspn(stringVal, "\n")] = 0;
                        addParameter(callStack, varName, VAR_STRING, stringVal);
                        break;
                    default:
                        printf("Invalid variable type!\n");
                }
                break;
                
            case 4:
                printf("Variable types: 0-int, 1-float, 2-char, 3-string\n");
                printf("Enter variable type (0-3): ");
                scanf("%d", &varType);
                getchar();
                
                printf("Enter variable name: ");
                fgets(varName, sizeof(varName), stdin);
                varName[strcspn(varName, "\n")] = 0;
                
                if (varType >= 0 && varType <= 3) {
                    addLocalVariable(callStack, varName, (VariableType)varType);
                } else {
                    printf("Invalid variable type!\n");
                }
                break;
                
            case 5:
                printf("Enter variable name to set: ");
                fgets(varName, sizeof(varName), stdin);
                varName[strcspn(varName, "\n")] = 0;
                
                if (!isFunctionStackEmpty(callStack)) {
                    Variable* var = findVariable(callStack->top->localVariables, varName);
                    if (var != NULL) {
                        switch (var->type) {
                            case VAR_INT:
                                printf("Enter integer value: ");
                                scanf("%d", &intVal);
                                setLocalVariableValue(callStack, varName, &intVal);
                                break;
                            case VAR_FLOAT:
                                printf("Enter float value: ");
                                scanf("%f", &floatVal);
                                setLocalVariableValue(callStack, varName, &floatVal);
                                break;
                            case VAR_CHAR:
                                printf("Enter char value: ");
                                scanf(" %c", &charVal);
                                setLocalVariableValue(callStack, varName, &charVal);
                                break;
                            case VAR_STRING:
                                getchar();
                                printf("Enter string value: ");
                                fgets(stringVal, sizeof(stringVal), stdin);
                                stringVal[strcspn(stringVal, "\n")] = 0;
                                setLocalVariableValue(callStack, varName, stringVal);
                                break;
                        }
                    } else {
                        printf("Variable '%s' not found!\n", varName);
                    }
                } else {
                    printf("No active function call!\n");
                }
                break;
                
            case 6:
                displayCurrentFrame(callStack);
                break;
                
            case 7:
                displayCallStack(callStack);
                break;
                
            case 8:
                printf("Enter number for factorial: ");
                scanf("%d", &intVal);
                if (intVal >= 0 && intVal <= 10) {
                    simulateFactorial(callStack, intVal);
                } else {
                    printf("Please enter a number between 0 and 10\n");
                }
                break;
                
            case 9:
                simulateNestedFunctions(callStack);
                break;
                
            case 10:
                printf("Enter depth to test (warning: large numbers may cause issues): ");
                scanf("%d", &intVal);
                if (intVal > 0 && intVal <= 200) {
                    simulateStackOverflow(callStack, intVal);
                } else {
                    printf("Please enter a reasonable depth (1-200)\n");
                }
                break;
                
            case 11:
                while (!isFunctionStackEmpty(callStack)) {
                    StackFrame* frame = popFunctionCall(callStack);
                    if (frame != NULL) {
                        freeVariableList(frame->parameters);
                        freeVariableList(frame->localVariables);
                        free(frame);
                    }
                }
                printf("All function calls cleared!\n");
                break;
                
            case 12:
                printf("Thanks for exploring Function Call Stack!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 12);
    
    freeFunctionCallStack(callStack);
    return 0;
}