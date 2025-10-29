/*
 * String Reversal using Stack Implementation
 * This program demonstrates string reversal using stack data structure
 * with various string processing features.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 1000

// Structure to represent a character stack node
typedef struct StackNode {
    char data;
    struct StackNode* next;
} StackNode;

// Structure to represent the stack
typedef struct Stack {
    StackNode* top;
    int size;
    int maxSize;
} Stack;

// Function to create a new stack
Stack* createStack(int maxSize) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    stack->maxSize = maxSize;
    return stack;
}

// Function to check if stack is empty
int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Function to check if stack is full
int isFull(Stack* stack) {
    return stack->size >= stack->maxSize;
}

// Function to push a character onto the stack
int push(Stack* stack, char ch) {
    if (isFull(stack)) {
        printf("Stack overflow! Cannot push '%c'\n", ch);
        return 0;
    }
    
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = ch;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
    return 1;
}

// Function to pop a character from the stack
char pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow! Cannot pop from empty stack\n");
        return '\0';
    }
    
    StackNode* temp = stack->top;
    char data = temp->data;
    stack->top = stack->top->next;
    stack->size--;
    free(temp);
    return data;
}

// Function to peek at the top character without removing it
char peek(Stack* stack) {
    if (isEmpty(stack)) {
        return '\0';
    }
    return stack->top->data;
}

// Function to display the stack contents
void displayStack(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return;
    }
    
    printf("Stack contents (top to bottom): ");
    StackNode* current = stack->top;
    while (current != NULL) {
        printf("'%c' ", current->data);
        current = current->next;
    }
    printf("\nStack size: %d\n", stack->size);
}

// Function to reverse a string using stack
char* reverseString(char* str) {
    int len = strlen(str);
    Stack* stack = createStack(len);
    
    // Push all characters onto the stack
    printf("Pushing characters: ");
    for (int i = 0; i < len; i++) {
        printf("'%c' ", str[i]);
        push(stack, str[i]);
    }
    printf("\n");
    
    // Create result string
    char* reversed = (char*)malloc((len + 1) * sizeof(char));
    
    // Pop all characters to get reversed string
    printf("Popping characters: ");
    for (int i = 0; i < len; i++) {
        reversed[i] = pop(stack);
        printf("'%c' ", reversed[i]);
    }
    printf("\n");
    
    reversed[len] = '\0';
    
    // Free the stack
    free(stack);
    return reversed;
}

// Function to reverse words in a sentence (keep word order, reverse each word)
char* reverseWordsInSentence(char* sentence) {
    int len = strlen(sentence);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    Stack* stack = createStack(len);
    
    int resultIndex = 0;
    
    printf("Processing sentence word by word:\n");
    
    for (int i = 0; i <= len; i++) {
        if (sentence[i] != ' ' && sentence[i] != '\0') {
            // Push character to stack
            push(stack, sentence[i]);
        } else {
            // End of word - pop all characters
            printf("Reversing word: ");
            while (!isEmpty(stack)) {
                char ch = pop(stack);
                printf("%c", ch);
                result[resultIndex++] = ch;
            }
            printf("\n");
            
            // Add space if not end of string
            if (sentence[i] == ' ') {
                result[resultIndex++] = ' ';
            }
        }
    }
    
    result[resultIndex] = '\0';
    free(stack);
    return result;
}

// Function to reverse only alphabetic characters in a string
char* reverseAlphabeticOnly(char* str) {
    int len = strlen(str);
    Stack* stack = createStack(len);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    
    // Push only alphabetic characters
    printf("Pushing alphabetic characters: ");
    for (int i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            printf("'%c' ", str[i]);
            push(stack, str[i]);
        }
    }
    printf("\n");
    
    // Rebuild string with reversed alphabetic characters
    for (int i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            result[i] = pop(stack);
        } else {
            result[i] = str[i]; // Keep non-alphabetic characters in place
        }
    }
    
    result[len] = '\0';
    free(stack);
    return result;
}

// Function to check if a string is palindrome using stack
int isPalindrome(char* str) {
    int len = strlen(str);
    Stack* stack = createStack(len);
    
    // Convert to lowercase and push only alphanumeric characters
    char* cleanStr = (char*)malloc(len + 1);
    int cleanLen = 0;
    
    for (int i = 0; i < len; i++) {
        if (isalnum(str[i])) {
            char ch = tolower(str[i]);
            cleanStr[cleanLen++] = ch;
            push(stack, ch);
        }
    }
    cleanStr[cleanLen] = '\0';
    
    printf("Cleaned string: %s\n", cleanStr);
    
    // Check if palindrome by comparing with popped characters
    for (int i = 0; i < cleanLen; i++) {
        if (cleanStr[i] != pop(stack)) {
            free(cleanStr);
            free(stack);
            return 0; // Not a palindrome
        }
    }
    
    free(cleanStr);
    free(stack);
    return 1; // Is a palindrome
}

// Function to reverse a string with preserving case pattern
char* reverseCasePreserving(char* str) {
    int len = strlen(str);
    Stack* upperStack = createStack(len);
    Stack* lowerStack = createStack(len);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    
    // Separate uppercase and lowercase letters
    for (int i = 0; i < len; i++) {
        if (isupper(str[i])) {
            push(upperStack, str[i]);
        } else if (islower(str[i])) {
            push(lowerStack, str[i]);
        }
    }
    
    // Rebuild string with reversed letters maintaining case pattern
    for (int i = 0; i < len; i++) {
        if (isupper(str[i])) {
            result[i] = isEmpty(upperStack) ? str[i] : pop(upperStack);
        } else if (islower(str[i])) {
            result[i] = isEmpty(lowerStack) ? str[i] : pop(lowerStack);
        } else {
            result[i] = str[i];
        }
    }
    
    result[len] = '\0';
    free(upperStack);
    free(lowerStack);
    return result;
}

// Function to reverse vowels only in a string
char* reverseVowelsOnly(char* str) {
    int len = strlen(str);
    Stack* stack = createStack(len);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    
    // Push vowels onto stack
    printf("Pushing vowels: ");
    for (int i = 0; i < len; i++) {
        char ch = tolower(str[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            printf("'%c' ", str[i]);
            push(stack, str[i]);
        }
    }
    printf("\n");
    
    // Rebuild string with reversed vowels
    for (int i = 0; i < len; i++) {
        char ch = tolower(str[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            result[i] = pop(stack);
        } else {
            result[i] = str[i];
        }
    }
    
    result[len] = '\0';
    free(stack);
    return result;
}

// Function to free the stack
void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

// Function to demonstrate stack operations
void demonstrateStackOperations() {
    printf("\n=== STACK OPERATIONS DEMO ===\n");
    Stack* stack = createStack(10);
    
    char testString[] = "HELLO";
    printf("Demonstrating with string: %s\n\n", testString);
    
    // Push characters
    printf("Pushing characters:\n");
    for (int i = 0; i < strlen(testString); i++) {
        printf("Push '%c'\n", testString[i]);
        push(stack, testString[i]);
        displayStack(stack);
        printf("\n");
    }
    
    // Peek at top
    printf("Peek at top: '%c'\n\n", peek(stack));
    
    // Pop characters
    printf("Popping characters:\n");
    while (!isEmpty(stack)) {
        char ch = pop(stack);
        printf("Popped '%c'\n", ch);
        displayStack(stack);
        printf("\n");
    }
    
    freeStack(stack);
}

// Main function with menu-driven interface
int main() {
    int choice;
    char input[1000];
    char* result;
    
    printf("🔤 Welcome to String Reversal using Stack! 🔤\n");
    printf("Explore various string reversal techniques!\n\n");
    
    do {
        printf("\n=== STRING REVERSAL MENU ===\n");
        printf("1. Reverse entire string\n");
        printf("2. Reverse each word in sentence\n");
        printf("3. Reverse alphabetic characters only\n");
        printf("4. Check if string is palindrome\n");
        printf("5. Reverse with case pattern preserved\n");
        printf("6. Reverse vowels only\n");
        printf("7. Demonstrate stack operations\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter string to reverse: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // remove newline
                
                printf("\nOriginal string: %s\n", input);
                result = reverseString(input);
                printf("Reversed string: %s\n", result);
                free(result);
                break;
                
            case 2:
                printf("Enter sentence (words will be reversed individually): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\nOriginal sentence: %s\n", input);
                result = reverseWordsInSentence(input);
                printf("Words reversed: %s\n", result);
                free(result);
                break;
                
            case 3:
                printf("Enter string (only alphabetic chars will be reversed): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\nOriginal string: %s\n", input);
                result = reverseAlphabeticOnly(input);
                printf("Alphabetic reversed: %s\n", result);
                free(result);
                break;
                
            case 4:
                printf("Enter string to check palindrome: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\nChecking: %s\n", input);
                if (isPalindrome(input)) {
                    printf("✓ This is a palindrome!\n");
                } else {
                    printf("✗ This is not a palindrome.\n");
                }
                break;
                
            case 5:
                printf("Enter string (case pattern will be preserved): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\nOriginal string: %s\n", input);
                result = reverseCasePreserving(input);
                printf("Case-preserved reverse: %s\n", result);
                free(result);
                break;
                
            case 6:
                printf("Enter string (only vowels will be reversed): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\nOriginal string: %s\n", input);
                result = reverseVowelsOnly(input);
                printf("Vowels reversed: %s\n", result);
                free(result);
                break;
                
            case 7:
                demonstrateStackOperations();
                break;
                
            case 8:
                printf("Thanks for exploring String Reversal with Stack!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
    
    return 0;
}