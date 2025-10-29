/*
 * Balanced Parentheses Checker using Stack
 * This program checks if parentheses, brackets, and braces are properly
 * balanced in expressions using stack data structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPRESSION_SIZE 1000

// Structure to represent a character stack node
typedef struct StackNode {
    char data;
    int position; // Position in the original expression
    struct StackNode* next;
} StackNode;

// Structure to represent the stack
typedef struct Stack {
    StackNode* top;
    int size;
} Stack;

// Structure to represent expression analysis
typedef struct ExpressionAnalysis {
    char expression[MAX_EXPRESSION_SIZE];
    int isBalanced;
    int totalOpenBrackets;
    int totalCloseBrackets;
    int maxDepth;
    int currentDepth;
    char errorDetails[500];
} ExpressionAnalysis;

// Function to create a new stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

// Function to check if stack is empty
int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Function to push a character with position onto the stack
void push(Stack* stack, char ch, int position) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = ch;
    newNode->position = position;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

// Function to pop a character from the stack
StackNode* pop(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    
    StackNode* temp = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    return temp;
}

// Function to peek at the top character without removing it
char peek(Stack* stack) {
    if (isEmpty(stack)) {
        return '\0';
    }
    return stack->top->data;
}

// Function to check if character is an opening bracket
int isOpeningBracket(char ch) {
    return (ch == '(' || ch == '[' || ch == '{');
}

// Function to check if character is a closing bracket
int isClosingBracket(char ch) {
    return (ch == ')' || ch == ']' || ch == '}');
}

// Function to get matching closing bracket for opening bracket
char getMatchingClosing(char opening) {
    switch (opening) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        default: return '\0';
    }
}

// Function to get matching opening bracket for closing bracket
char getMatchingOpening(char closing) {
    switch (closing) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default: return '\0';
    }
}

// Function to get bracket type name
const char* getBracketName(char bracket) {
    switch (bracket) {
        case '(': case ')': return "parenthesis";
        case '[': case ']': return "square bracket";
        case '{': case '}': return "curly brace";
        default: return "unknown";
    }
}

// Function to check if brackets match
int isMatchingPair(char opening, char closing) {
    return ((opening == '(' && closing == ')') ||
            (opening == '[' && closing == ']') ||
            (opening == '{' && closing == '}'));
}

// Function to analyze expression for balanced parentheses
ExpressionAnalysis* analyzeExpression(char* expression) {
    ExpressionAnalysis* analysis = (ExpressionAnalysis*)malloc(sizeof(ExpressionAnalysis));
    strcpy(analysis->expression, expression);
    analysis->isBalanced = 1;
    analysis->totalOpenBrackets = 0;
    analysis->totalCloseBrackets = 0;
    analysis->maxDepth = 0;
    analysis->currentDepth = 0;
    strcpy(analysis->errorDetails, "");
    
    Stack* stack = createStack();
    int len = strlen(expression);
    
    printf("\n=== ANALYZING EXPRESSION ===\n");
    printf("Expression: %s\n", expression);
    printf("Processing character by character:\n\n");
    
    for (int i = 0; i < len; i++) {
        char ch = expression[i];
        
        if (isOpeningBracket(ch)) {
            printf("Position %d: '%c' - Opening %s, pushing to stack\n", 
                   i, ch, getBracketName(ch));
            
            push(stack, ch, i);
            analysis->totalOpenBrackets++;
            analysis->currentDepth++;
            
            if (analysis->currentDepth > analysis->maxDepth) {
                analysis->maxDepth = analysis->currentDepth;
            }
            
        } else if (isClosingBracket(ch)) {
            printf("Position %d: '%c' - Closing %s", 
                   i, ch, getBracketName(ch));
            
            analysis->totalCloseBrackets++;
            
            if (isEmpty(stack)) {
                printf(" - ERROR: No matching opening bracket!\n");
                analysis->isBalanced = 0;
                sprintf(analysis->errorDetails, 
                        "Unmatched closing %s '%c' at position %d", 
                        getBracketName(ch), ch, i);
                break;
            }
            
            StackNode* topNode = pop(stack);
            char openingBracket = topNode->data;
            int openingPosition = topNode->position;
            
            if (isMatchingPair(openingBracket, ch)) {
                printf(" - MATCH with '%c' at position %d\n", 
                       openingBracket, openingPosition);
                analysis->currentDepth--;
            } else {
                printf(" - ERROR: Mismatched with '%c' at position %d\n", 
                       openingBracket, openingPosition);
                analysis->isBalanced = 0;
                sprintf(analysis->errorDetails, 
                        "Mismatched brackets: '%c' at position %d and '%c' at position %d", 
                        openingBracket, openingPosition, ch, i);
                free(topNode);
                break;
            }
            
            free(topNode);
            
        } else {
            // Regular character - just note it
            if (ch != ' ' && ch != '\t' && ch != '\n') {
                printf("Position %d: '%c' - Regular character\n", i, ch);
            }
        }
    }
    
    // Check if there are unmatched opening brackets
    if (analysis->isBalanced && !isEmpty(stack)) {
        printf("\nERROR: Unmatched opening brackets remaining:\n");
        while (!isEmpty(stack)) {
            StackNode* node = pop(stack);
            printf("- '%c' at position %d\n", node->data, node->position);
            free(node);
        }
        analysis->isBalanced = 0;
        strcpy(analysis->errorDetails, "Unmatched opening brackets at end of expression");
    }
    
    // Free remaining stack
    while (!isEmpty(stack)) {
        StackNode* node = pop(stack);
        free(node);
    }
    free(stack);
    
    return analysis;
}

// Function to display analysis results
void displayAnalysisResults(ExpressionAnalysis* analysis) {
    printf("\n=== ANALYSIS RESULTS ===\n");
    printf("Expression: %s\n", analysis->expression);
    
    if (analysis->isBalanced) {
        printf("BALANCED: All brackets are properly matched!\n");
    } else {
        printf("NOT BALANCED: Bracket mismatch found!\n");
        printf("Error: %s\n", analysis->errorDetails);
    }
    
    printf("\nStatistics:\n");
    printf("- Total opening brackets: %d\n", analysis->totalOpenBrackets);
    printf("- Total closing brackets: %d\n", analysis->totalCloseBrackets);
    printf("- Maximum nesting depth: %d\n", analysis->maxDepth);
    printf("- Expression length: %ld characters\n", strlen(analysis->expression));
    
    // Show bracket distribution
    int parentheses = 0, squares = 0, braces = 0;
    for (int i = 0; i < strlen(analysis->expression); i++) {
        char ch = analysis->expression[i];
        if (ch == '(' || ch == ')') parentheses++;
        else if (ch == '[' || ch == ']') squares++;
        else if (ch == '{' || ch == '}') braces++;
    }
    
    printf("\nBracket distribution:\n");
    printf("- Parentheses ( ): %d\n", parentheses);
    printf("- Square brackets [ ]: %d\n", squares);
    printf("- Curly braces { }: %d\n", braces);
    printf("========================\n");
}

// Function to check simple balanced parentheses (only round brackets)
int checkSimpleParentheses(char* expression) {
    Stack* stack = createStack();
    int len = strlen(expression);
    
    printf("\nChecking simple parentheses in: %s\n", expression);
    
    for (int i = 0; i < len; i++) {
        if (expression[i] == '(') {
            push(stack, '(', i);
            printf("'(' at position %d - pushed\n", i);
        } else if (expression[i] == ')') {
            if (isEmpty(stack)) {
                printf("')' at position %d - ERROR: no matching '('\n", i);
                free(stack);
                return 0;
            }
            StackNode* node = pop(stack);
            printf("')' at position %d - matched with '(' at position %d\n", i, node->position);
            free(node);
        }
    }
    
    int result = isEmpty(stack);
    
    if (!result) {
        printf("ERROR: Unmatched opening parentheses:\n");
        while (!isEmpty(stack)) {
            StackNode* node = pop(stack);
            printf("- '(' at position %d\n", node->position);
            free(node);
        }
    }
    
    free(stack);
    return result;
}

// Function to validate mathematical expression
int validateMathExpression(char* expression) {
    printf("\n=== VALIDATING MATHEMATICAL EXPRESSION ===\n");
    printf("Expression: %s\n", expression);
    
    // Check balanced brackets
    ExpressionAnalysis* analysis = analyzeExpression(expression);
    
    if (!analysis->isBalanced) {
        printf("Invalid: Brackets are not balanced\n");
        free(analysis);
        return 0;
    }
    
    // Additional checks for mathematical validity
    int len = strlen(expression);
    int openParenCount = 0;
    
    for (int i = 0; i < len; i++) {
        char ch = expression[i];
        
        if (ch == '(') {
            openParenCount++;
        } else if (ch == ')') {
            openParenCount--;
            
            // Check for empty parentheses
            if (i > 0 && expression[i-1] == '(') {
                printf("❌ Invalid: Empty parentheses found at position %d\n", i-1);
                free(analysis);
                return 0;
            }
        }
    }
    
    printf("✅ Valid mathematical expression with balanced brackets\n");
    free(analysis);
    return 1;
}

// Function to show bracket depth visualization
void visualizeBracketDepth(char* expression) {
    printf("\n=== BRACKET DEPTH VISUALIZATION ===\n");
    printf("Expression: %s\n", expression);
    printf("Depth:      ");
    
    Stack* stack = createStack();
    int len = strlen(expression);
    
    for (int i = 0; i < len; i++) {
        char ch = expression[i];
        
        if (isOpeningBracket(ch)) {
            push(stack, ch, i);
            printf("%d", stack->size);
        } else if (isClosingBracket(ch)) {
            if (!isEmpty(stack)) {
                StackNode* node = pop(stack);
                printf("%d", stack->size);
                free(node);
            } else {
                printf("X"); // Error indicator
            }
        } else {
            printf("%d", stack->size);
        }
    }
    
    // Cleanup
    while (!isEmpty(stack)) {
        StackNode* node = pop(stack);
        free(node);
    }
    free(stack);
    
    printf("\n\nLegend: Numbers show nesting depth, X indicates error\n");
}

// Function to test with sample expressions
void testSampleExpressions() {
    printf("\n=== TESTING SAMPLE EXPRESSIONS ===\n");
    
    char* samples[] = {
        "()",
        "(())",
        "()[]{}", 
        "([{}])",
        "((()))",
        "({[]})",
        ")(",
        "([)]",
        "(()",
        "({[}])",
        "(a+b)*[c-d]",
        "{[a*(b+c)]-d}",
        "((a+b)*(c-d)",
        "if(x>0){while(i<n){arr[i++]=0;}}",
        "function(){return [1,2,3];}"
    };
    
    int numSamples = sizeof(samples) / sizeof(samples[0]);
    
    for (int i = 0; i < numSamples; i++) {
        printf("\n--- Test %d ---\n", i + 1);
        ExpressionAnalysis* analysis = analyzeExpression(samples[i]);
        printf("Result: %s\n", analysis->isBalanced ? "BALANCED" : "NOT BALANCED");
        free(analysis);
    }
}

// Function to free analysis memory
void freeAnalysis(ExpressionAnalysis* analysis) {
    free(analysis);
}

// Main function with menu-driven interface
int main() {
    int choice;
    char expression[MAX_EXPRESSION_SIZE];
    ExpressionAnalysis* analysis;
    
    printf("Welcome to Balanced Parentheses Checker!\n");
    printf("Check if brackets are properly balanced using stack!\n\n");
    
    do {
        printf("\n=== BRACKET CHECKER MENU ===\n");
        printf("1. Check balanced brackets (all types)\n");
        printf("2. Check simple parentheses only\n");
        printf("3. Validate mathematical expression\n");
        printf("4. Visualize bracket depth\n");
        printf("5. Test sample expressions\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter expression to check: ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = 0; // remove newline
                
                analysis = analyzeExpression(expression);
                displayAnalysisResults(analysis);
                freeAnalysis(analysis);
                break;
                
            case 2:
                printf("Enter expression with parentheses: ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = 0;
                
                if (checkSimpleParentheses(expression)) {
                    printf("Parentheses are balanced!\n");
                } else {
                    printf("Parentheses are NOT balanced!\n");
                }
                break;
                
            case 3:
                printf("Enter mathematical expression: ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = 0;
                
                validateMathExpression(expression);
                break;
                
            case 4:
                printf("Enter expression to visualize: ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = 0;
                
                visualizeBracketDepth(expression);
                break;
                
            case 5:
                testSampleExpressions();
                break;
                
            case 6:
                printf("Thanks for using Balanced Parentheses Checker!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
    
    return 0;
}