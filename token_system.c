/*
 * Token Number Announcement System for Banks and Hospitals
 * This program implements a queue-based token management system
 * for service announcements in banks and hospitals.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_SERVICE_NAME 30
#define MAX_QUEUE_SIZE 100

// Enumeration for token types
typedef enum {
    TOKEN_GENERAL = 1,
    TOKEN_PREMIUM = 2,
    TOKEN_SENIOR = 3,
    TOKEN_EMERGENCY = 4
} TokenType;

// Enumeration for service status
typedef enum {
    STATUS_WAITING,
    STATUS_CALLED,
    STATUS_SERVING,
    STATUS_COMPLETED
} ServiceStatus;

// Structure to represent a token/customer
typedef struct Token {
    int tokenNumber;
    char customerName[MAX_NAME_LENGTH];
    TokenType type;
    char serviceName[MAX_SERVICE_NAME];
    time_t issueTime;
    time_t callTime;
    time_t serviceTime;
    ServiceStatus status;
    int estimatedWaitTime; // in minutes
    struct Token* next;
} Token;

// Structure to represent a service counter
typedef struct ServiceCounter {
    int counterId;
    char counterName[MAX_SERVICE_NAME];
    Token* currentToken;
    int isActive;
    int totalServed;
    float avgServiceTime; // in minutes
} ServiceCounter;

// Structure to represent the token queue system
typedef struct TokenQueue {
    Token* front;
    Token* rear;
    int size;
    int nextTokenNumber;
    ServiceCounter counters[10];
    int counterCount;
    int totalTokensIssued;
    int totalTokensServed;
} TokenQueue;

// Function to get token type string
const char* getTokenTypeString(TokenType type) {
    switch (type) {
        case TOKEN_GENERAL: return "General";
        case TOKEN_PREMIUM: return "Premium";
        case TOKEN_SENIOR: return "Senior Citizen";
        case TOKEN_EMERGENCY: return "Emergency";
        default: return "Unknown";
    }
}

// Function to get status string
const char* getStatusString(ServiceStatus status) {
    switch (status) {
        case STATUS_WAITING: return "Waiting";
        case STATUS_CALLED: return "Called";
        case STATUS_SERVING: return "Being Served";
        case STATUS_COMPLETED: return "Completed";
        default: return "Unknown";
    }
}

// Function to create a token queue system
TokenQueue* createTokenQueue() {
    TokenQueue* queue = (TokenQueue*)malloc(sizeof(TokenQueue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    queue->nextTokenNumber = 1;
    queue->counterCount = 0;
    queue->totalTokensIssued = 0;
    queue->totalTokensServed = 0;
    return queue;
}

// Function to check if queue is empty
int isQueueEmpty(TokenQueue* queue) {
    return queue->front == NULL;
}

// Function to check if queue is full
int isQueueFull(TokenQueue* queue) {
    return queue->size >= MAX_QUEUE_SIZE;
}

// Function to add service counter
void addServiceCounter(TokenQueue* queue, char* counterName) {
    if (queue->counterCount >= 10) {
        printf("❌ Maximum number of service counters reached!\n");
        return;
    }
    
    ServiceCounter* counter = &queue->counters[queue->counterCount];
    counter->counterId = queue->counterCount + 1;
    strcpy(counter->counterName, counterName);
    counter->currentToken = NULL;
    counter->isActive = 1;
    counter->totalServed = 0;
    counter->avgServiceTime = 5.0f; // Default 5 minutes
    
    queue->counterCount++;
    printf("✅ Service counter '%s' (ID: %d) added successfully!\n", 
           counterName, counter->counterId);
}

// Function to issue a new token
Token* issueToken(TokenQueue* queue, char* customerName, TokenType type, char* serviceName) {
    if (isQueueFull(queue)) {
        printf("❌ Queue is full! Cannot issue more tokens.\n");
        return NULL;
    }
    
    Token* newToken = (Token*)malloc(sizeof(Token));
    newToken->tokenNumber = queue->nextTokenNumber++;
    strcpy(newToken->customerName, customerName);
    newToken->type = type;
    strcpy(newToken->serviceName, serviceName);
    newToken->issueTime = time(NULL);
    newToken->callTime = 0;
    newToken->serviceTime = 0;
    newToken->status = STATUS_WAITING;
    newToken->next = NULL;
    
    // Calculate estimated wait time based on queue position and type
    int position = queue->size;
    if (type == TOKEN_EMERGENCY) {
        position = 0; // Emergency tokens go to front
    } else if (type == TOKEN_SENIOR || type == TOKEN_PREMIUM) {
        position = position / 2; // Priority tokens get better position
    }
    newToken->estimatedWaitTime = position * 5; // 5 minutes average per person
    
    // Add to queue based on priority
    if (isQueueEmpty(queue) || type != TOKEN_EMERGENCY) {
        // Normal enqueue
        if (isQueueEmpty(queue)) {
            queue->front = queue->rear = newToken;
        } else {
            queue->rear->next = newToken;
            queue->rear = newToken;
        }
    } else {
        // Emergency token - add to front
        newToken->next = queue->front;
        queue->front = newToken;
        if (queue->rear == NULL) {
            queue->rear = newToken;
        }
    }
    
    queue->size++;
    queue->totalTokensIssued++;
    
    printf("🎫 Token issued successfully!\n");
    printf("   Token Number: %d\n", newToken->tokenNumber);
    printf("   Customer: %s\n", customerName);
    printf("   Type: %s\n", getTokenTypeString(type));
    printf("   Service: %s\n", serviceName);
    printf("   Estimated Wait: %d minutes\n", newToken->estimatedWaitTime);
    
    return newToken;
}

// Function to call next token for service
Token* callNextToken(TokenQueue* queue, int counterId) {
    if (isQueueEmpty(queue)) {
        printf("❌ No tokens in queue to call!\n");
        return NULL;
    }
    
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return NULL;
    }
    
    ServiceCounter* counter = &queue->counters[counterId - 1];
    if (!counter->isActive) {
        printf("❌ Counter %d is not active!\n", counterId);
        return NULL;
    }
    
    if (counter->currentToken != NULL && counter->currentToken->status == STATUS_SERVING) {
        printf("❌ Counter %d is currently serving token %d!\n", 
               counterId, counter->currentToken->tokenNumber);
        return NULL;
    }
    
    // Dequeue the front token
    Token* token = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    queue->size--;
    
    // Update token status
    token->status = STATUS_CALLED;
    token->callTime = time(NULL);
    token->next = NULL;
    
    // Assign to counter
    counter->currentToken = token;
    
    printf("📢 ANNOUNCEMENT: Token Number %d (%s - %s) please proceed to Counter %d (%s)\n",
           token->tokenNumber, token->customerName, getTokenTypeString(token->type),
           counterId, counter->counterName);
    
    return token;
}

// Function to start serving a token
void startService(TokenQueue* queue, int counterId) {
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return;
    }
    
    ServiceCounter* counter = &queue->counters[counterId - 1];
    
    if (counter->currentToken == NULL) {
        printf("❌ No token assigned to counter %d!\n", counterId);
        return;
    }
    
    if (counter->currentToken->status != STATUS_CALLED) {
        printf("❌ Token %d is not in called status!\n", counter->currentToken->tokenNumber);
        return;
    }
    
    counter->currentToken->status = STATUS_SERVING;
    counter->currentToken->serviceTime = time(NULL);
    
    printf("🔧 Service started for Token %d at Counter %d (%s)\n",
           counter->currentToken->tokenNumber, counterId, counter->counterName);
}

// Function to complete service for a token
void completeService(TokenQueue* queue, int counterId) {
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return;
    }
    
    ServiceCounter* counter = &queue->counters[counterId - 1];
    
    if (counter->currentToken == NULL) {
        printf("❌ No token being served at counter %d!\n", counterId);
        return;
    }
    
    Token* token = counter->currentToken;
    token->status = STATUS_COMPLETED;
    
    // Calculate service time
    time_t completionTime = time(NULL);
    float serviceTime = difftime(completionTime, token->serviceTime) / 60.0f; // in minutes
    
    // Update counter statistics
    counter->totalServed++;
    counter->avgServiceTime = ((counter->avgServiceTime * (counter->totalServed - 1)) + serviceTime) / counter->totalServed;
    
    queue->totalTokensServed++;
    
    printf("✅ Service completed for Token %d (Customer: %s)\n", 
           token->tokenNumber, token->customerName);
    printf("   Service time: %.1f minutes\n", serviceTime);
    printf("   Total wait time: %.1f minutes\n", 
           difftime(completionTime, token->issueTime) / 60.0f);
    
    // Free the token and reset counter
    free(token);
    counter->currentToken = NULL;
    
    printf("   Counter %d is now available for next customer\n", counterId);
}

// Function to display current queue status
void displayQueueStatus(TokenQueue* queue) {
    printf("\n=== TOKEN QUEUE STATUS ===\n");
    printf("Total tokens in queue: %d\n", queue->size);
    printf("Next token number: %d\n", queue->nextTokenNumber);
    printf("Total tokens issued: %d\n", queue->totalTokensIssued);
    printf("Total tokens served: %d\n", queue->totalTokensServed);
    
    if (isQueueEmpty(queue)) {
        printf("Queue is empty - no customers waiting\n");
    } else {
        printf("\nWaiting customers:\n");
        printf("%-8s %-20s %-15s %-20s %-10s\n", 
               "Token", "Customer", "Type", "Service", "Wait(min)");
        printf("------------------------------------------------------------------------\n");
        
        Token* current = queue->front;
        while (current != NULL) {
            printf("%-8d %-20s %-15s %-20s %-10d\n",
                   current->tokenNumber,
                   current->customerName,
                   getTokenTypeString(current->type),
                   current->serviceName,
                   current->estimatedWaitTime);
            current = current->next;
        }
    }
    printf("========================\n\n");
}

// Function to display counter status
void displayCounterStatus(TokenQueue* queue) {
    if (queue->counterCount == 0) {
        printf("No service counters configured!\n");
        return;
    }
    
    printf("\n=== SERVICE COUNTER STATUS ===\n");
    printf("%-10s %-20s %-10s %-20s %-12s %-10s\n",
           "Counter", "Name", "Status", "Current Token", "Avg Service", "Served");
    printf("------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < queue->counterCount; i++) {
        ServiceCounter* counter = &queue->counters[i];
        char status[20], tokenInfo[30];
        
        if (!counter->isActive) {
            strcpy(status, "Inactive");
            strcpy(tokenInfo, "N/A");
        } else if (counter->currentToken == NULL) {
            strcpy(status, "Available");
            strcpy(tokenInfo, "None");
        } else {
            strcpy(status, "Busy");
            sprintf(tokenInfo, "%d (%s)", 
                    counter->currentToken->tokenNumber,
                    getStatusString(counter->currentToken->status));
        }
        
        printf("%-10d %-20s %-10s %-20s %-12.1f %-10d\n",
               counter->counterId,
               counter->counterName,
               status,
               tokenInfo,
               counter->avgServiceTime,
               counter->totalServed);
    }
    printf("===============================\n\n");
}

// Function to display announcements
void displayAnnouncements(TokenQueue* queue) {
    printf("\n=== CURRENT ANNOUNCEMENTS ===\n");
    
    int announcements = 0;
    for (int i = 0; i < queue->counterCount; i++) {
        ServiceCounter* counter = &queue->counters[i];
        if (counter->currentToken != NULL) {
            Token* token = counter->currentToken;
            
            if (token->status == STATUS_CALLED) {
                printf("📢 CALLING: Token %d (%s - %s) to Counter %d (%s)\n",
                       token->tokenNumber, token->customerName, 
                       getTokenTypeString(token->type),
                       counter->counterId, counter->counterName);
                announcements++;
            } else if (token->status == STATUS_SERVING) {
                printf("🔧 SERVING: Token %d at Counter %d (%s)\n",
                       token->tokenNumber, counter->counterId, counter->counterName);
                announcements++;
            }
        }
    }
    
    if (announcements == 0) {
        printf("No current announcements\n");
    }
    printf("=============================\n\n");
}

// Function to activate/deactivate counter
void toggleCounter(TokenQueue* queue, int counterId) {
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return;
    }
    
    ServiceCounter* counter = &queue->counters[counterId - 1];
    
    if (counter->isActive && counter->currentToken != NULL) {
        printf("❌ Cannot deactivate counter %d - currently serving a customer!\n", counterId);
        return;
    }
    
    counter->isActive = !counter->isActive;
    printf("%s Counter %d (%s)\n", 
           counter->isActive ? "🟢 Activated" : "🔴 Deactivated",
           counterId, counter->counterName);
}

// Function to setup sample data
void setupSampleData(TokenQueue* queue) {
    printf("Setting up sample data...\n");
    
    // Add service counters
    addServiceCounter(queue, "General Service");
    addServiceCounter(queue, "Account Opening");
    addServiceCounter(queue, "Loan Processing");
    addServiceCounter(queue, "Premium Service");
    
    // Issue sample tokens
    issueToken(queue, "John Smith", TOKEN_GENERAL, "Account Inquiry");
    issueToken(queue, "Mary Johnson", TOKEN_PREMIUM, "Investment Consultation");
    issueToken(queue, "Robert Brown", TOKEN_SENIOR, "Pension Processing");
    issueToken(queue, "Lisa Davis", TOKEN_GENERAL, "Money Transfer");
    issueToken(queue, "Emergency Case", TOKEN_EMERGENCY, "Urgent Transaction");
    issueToken(queue, "David Wilson", TOKEN_GENERAL, "Balance Inquiry");
    
    printf("Sample data setup complete!\n");
}

// Function to free the queue
void freeTokenQueue(TokenQueue* queue) {
    while (!isQueueEmpty(queue)) {
        Token* token = queue->front;
        queue->front = queue->front->next;
        free(token);
    }
    
    // Free any tokens still being served
    for (int i = 0; i < queue->counterCount; i++) {
        if (queue->counters[i].currentToken != NULL) {
            free(queue->counters[i].currentToken);
        }
    }
    
    free(queue);
}

// Main function with menu-driven interface
int main() {
    TokenQueue* tokenQueue = createTokenQueue();
    int choice, counterId, tokenType;
    char customerName[MAX_NAME_LENGTH], serviceName[MAX_SERVICE_NAME], counterName[MAX_SERVICE_NAME];
    
    printf("🏦 Welcome to Token Number Announcement System! 🏥\n");
    printf("Efficient queue management for Banks and Hospitals!\n\n");
    
    do {
        printf("\n=== TOKEN MANAGEMENT SYSTEM ===\n");
        printf("1. Issue new token\n");
        printf("2. Call next token\n");
        printf("3. Start service\n");
        printf("4. Complete service\n");
        printf("5. Display queue status\n");
        printf("6. Display counter status\n");
        printf("7. Display announcements\n");
        printf("8. Add service counter\n");
        printf("9. Toggle counter active/inactive\n");
        printf("10. Setup sample data\n");
        printf("11. Clear all data\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter customer name: ");
                fgets(customerName, sizeof(customerName), stdin);
                customerName[strcspn(customerName, "\n")] = 0;
                
                printf("Token types: 1-General, 2-Premium, 3-Senior, 4-Emergency\n");
                printf("Enter token type (1-4): ");
                scanf("%d", &tokenType);
                getchar();
                
                printf("Enter service name: ");
                fgets(serviceName, sizeof(serviceName), stdin);
                serviceName[strcspn(serviceName, "\n")] = 0;
                
                if (tokenType >= 1 && tokenType <= 4) {
                    issueToken(tokenQueue, customerName, (TokenType)tokenType, serviceName);
                } else {
                    printf("Invalid token type!\n");
                }
                break;
                
            case 2:
                printf("Enter counter ID to call next token: ");
                scanf("%d", &counterId);
                callNextToken(tokenQueue, counterId);
                break;
                
            case 3:
                printf("Enter counter ID to start service: ");
                scanf("%d", &counterId);
                startService(tokenQueue, counterId);
                break;
                
            case 4:
                printf("Enter counter ID to complete service: ");
                scanf("%d", &counterId);
                completeService(tokenQueue, counterId);
                break;
                
            case 5:
                displayQueueStatus(tokenQueue);
                break;
                
            case 6:
                displayCounterStatus(tokenQueue);
                break;
                
            case 7:
                displayAnnouncements(tokenQueue);
                break;
                
            case 8:
                printf("Enter counter name: ");
                fgets(counterName, sizeof(counterName), stdin);
                counterName[strcspn(counterName, "\n")] = 0;
                addServiceCounter(tokenQueue, counterName);
                break;
                
            case 9:
                printf("Enter counter ID to toggle: ");
                scanf("%d", &counterId);
                toggleCounter(tokenQueue, counterId);
                break;
                
            case 10:
                setupSampleData(tokenQueue);
                break;
                
            case 11:
                freeTokenQueue(tokenQueue);
                tokenQueue = createTokenQueue();
                printf("All data cleared!\n");
                break;
                
            case 12:
                printf("Thanks for using Token Management System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 12);
    
    freeTokenQueue(tokenQueue);
    return 0;
}