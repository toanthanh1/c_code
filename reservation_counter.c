/*
 * Reservation and Ticket Counter Model using Queue
 * This program implements a queue-based reservation and ticketing system
 * for theaters, airlines, trains, and event management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_EVENT_NAME 40
#define MAX_LOCATION_NAME 30
#define MAX_QUEUE_SIZE 200

// Enumeration for reservation types
typedef enum {
    RESERVATION_MOVIE = 1,
    RESERVATION_FLIGHT = 2,
    RESERVATION_TRAIN = 3,
    RESERVATION_EVENT = 4,
    RESERVATION_HOTEL = 5
} ReservationType;

// Enumeration for ticket status
typedef enum {
    TICKET_PENDING,
    TICKET_CONFIRMED,
    TICKET_CANCELLED,
    TICKET_CHECKED_IN,
    TICKET_COMPLETED
} TicketStatus;

// Enumeration for priority levels
typedef enum {
    PRIORITY_NORMAL = 1,
    PRIORITY_PREMIUM = 2,
    PRIORITY_VIP = 3,
    PRIORITY_EMERGENCY = 4
} PriorityLevel;

// Structure to represent a reservation request
typedef struct Reservation {
    int reservationId;
    char customerName[MAX_NAME_LENGTH];
    char email[MAX_NAME_LENGTH];
    char phoneNumber[20];
    ReservationType type;
    char eventName[MAX_EVENT_NAME];
    char location[MAX_LOCATION_NAME];
    char dateTime[30];
    int numberOfTickets;
    float totalAmount;
    PriorityLevel priority;
    TicketStatus status;
    time_t requestTime;
    time_t processTime;
    time_t confirmationTime;
    int estimatedWaitTime; // in minutes
    struct Reservation* next;
} Reservation;

// Structure to represent a service counter
typedef struct Counter {
    int counterId;
    char counterName[MAX_NAME_LENGTH];
    char operatorName[MAX_NAME_LENGTH];
    Reservation* currentReservation;
    int isActive;
    int totalProcessed;
    float avgProcessingTime; // in minutes
    ReservationType specialization; // 0 for all types
} Counter;

// Structure to represent the reservation queue system
typedef struct ReservationQueue {
    Reservation* front;
    Reservation* rear;
    int size;
    int nextReservationId;
    Counter counters[15];
    int counterCount;
    int totalReservations;
    int totalConfirmed;
    int totalCancelled;
    float totalRevenue;
} ReservationQueue;

// Function to get reservation type string
const char* getReservationTypeString(ReservationType type) {
    switch (type) {
        case RESERVATION_MOVIE: return "Movie";
        case RESERVATION_FLIGHT: return "Flight";
        case RESERVATION_TRAIN: return "Train";
        case RESERVATION_EVENT: return "Event";
        case RESERVATION_HOTEL: return "Hotel";
        default: return "Unknown";
    }
}

// Function to get ticket status string
const char* getTicketStatusString(TicketStatus status) {
    switch (status) {
        case TICKET_PENDING: return "Pending";
        case TICKET_CONFIRMED: return "Confirmed";
        case TICKET_CANCELLED: return "Cancelled";
        case TICKET_CHECKED_IN: return "Checked In";
        case TICKET_COMPLETED: return "Completed";
        default: return "Unknown";
    }
}

// Function to get priority string
const char* getPriorityString(PriorityLevel priority) {
    switch (priority) {
        case PRIORITY_NORMAL: return "Normal";
        case PRIORITY_PREMIUM: return "Premium";
        case PRIORITY_VIP: return "VIP";
        case PRIORITY_EMERGENCY: return "Emergency";
        default: return "Unknown";
    }
}

// Function to create a reservation queue system
ReservationQueue* createReservationQueue() {
    ReservationQueue* queue = (ReservationQueue*)malloc(sizeof(ReservationQueue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    queue->nextReservationId = 1001; // Start from 1001
    queue->counterCount = 0;
    queue->totalReservations = 0;
    queue->totalConfirmed = 0;
    queue->totalCancelled = 0;
    queue->totalRevenue = 0.0f;
    return queue;
}

// Function to check if queue is empty
int isReservationQueueEmpty(ReservationQueue* queue) {
    return queue->front == NULL;
}

// Function to check if queue is full
int isReservationQueueFull(ReservationQueue* queue) {
    return queue->size >= MAX_QUEUE_SIZE;
}

// Function to add service counter
void addCounter(ReservationQueue* queue, char* counterName, char* operatorName, ReservationType specialization) {
    if (queue->counterCount >= 15) {
        printf("❌ Maximum number of counters reached!\n");
        return;
    }
    
    Counter* counter = &queue->counters[queue->counterCount];
    counter->counterId = queue->counterCount + 1;
    strcpy(counter->counterName, counterName);
    strcpy(counter->operatorName, operatorName);
    counter->specialization = specialization;
    counter->currentReservation = NULL;
    counter->isActive = 1;
    counter->totalProcessed = 0;
    counter->avgProcessingTime = 3.0f; // Default 3 minutes
    
    queue->counterCount++;
    printf("✅ Counter '%s' (ID: %d) with operator '%s' added successfully!\n", 
           counterName, counter->counterId, operatorName);
    
    if (specialization != 0) {
        printf("   Specialization: %s reservations\n", getReservationTypeString(specialization));
    } else {
        printf("   Handles all types of reservations\n");
    }
}

// Function to calculate estimated wait time
int calculateWaitTime(ReservationQueue* queue, PriorityLevel priority) {
    int position = 0;
    Reservation* current = queue->front;
    
    while (current != NULL) {
        if (current->priority >= priority) {
            position++;
        }
        current = current->next;
    }
    
    // Base wait time calculation
    int baseTime = position * 4; // 4 minutes per reservation ahead
    
    switch (priority) {
        case PRIORITY_EMERGENCY: return baseTime * 0.2f;
        case PRIORITY_VIP: return baseTime * 0.5f;
        case PRIORITY_PREMIUM: return baseTime * 0.7f;
        default: return baseTime;
    }
}

// Function to add reservation to queue (priority-based insertion)
Reservation* addReservation(ReservationQueue* queue, char* customerName, char* email, char* phone,
                           ReservationType type, char* eventName, char* location, char* dateTime,
                           int numTickets, float amount, PriorityLevel priority) {
    
    if (isReservationQueueFull(queue)) {
        printf("❌ Reservation queue is full! Cannot accept more reservations.\n");
        return NULL;
    }
    
    Reservation* newReservation = (Reservation*)malloc(sizeof(Reservation));
    newReservation->reservationId = queue->nextReservationId++;
    strcpy(newReservation->customerName, customerName);
    strcpy(newReservation->email, email);
    strcpy(newReservation->phoneNumber, phone);
    newReservation->type = type;
    strcpy(newReservation->eventName, eventName);
    strcpy(newReservation->location, location);
    strcpy(newReservation->dateTime, dateTime);
    newReservation->numberOfTickets = numTickets;
    newReservation->totalAmount = amount;
    newReservation->priority = priority;
    newReservation->status = TICKET_PENDING;
    newReservation->requestTime = time(NULL);
    newReservation->processTime = 0;
    newReservation->confirmationTime = 0;
    newReservation->estimatedWaitTime = calculateWaitTime(queue, priority);
    newReservation->next = NULL;
    
    // Priority-based insertion
    if (isReservationQueueEmpty(queue)) {
        queue->front = queue->rear = newReservation;
    } else if (priority == PRIORITY_EMERGENCY || 
               (priority == PRIORITY_VIP && queue->front->priority < PRIORITY_VIP)) {
        // Insert at front for emergency or VIP (if front is not emergency)
        newReservation->next = queue->front;
        queue->front = newReservation;
    } else {
        // Find appropriate position based on priority
        Reservation* current = queue->front;
        Reservation* previous = NULL;
        
        while (current != NULL && current->priority >= priority) {
            previous = current;
            current = current->next;
        }
        
        if (previous == NULL) {
            // Insert at front
            newReservation->next = queue->front;
            queue->front = newReservation;
        } else if (current == NULL) {
            // Insert at rear
            previous->next = newReservation;
            queue->rear = newReservation;
        } else {
            // Insert in middle
            previous->next = newReservation;
            newReservation->next = current;
        }
    }
    
    queue->size++;
    queue->totalReservations++;
    
    printf("🎫 Reservation created successfully!\n");
    printf("   Reservation ID: %d\n", newReservation->reservationId);
    printf("   Customer: %s\n", customerName);
    printf("   Type: %s\n", getReservationTypeString(type));
    printf("   Event: %s\n", eventName);
    printf("   Tickets: %d\n", numTickets);
    printf("   Amount: $%.2f\n", amount);
    printf("   Priority: %s\n", getPriorityString(priority));
    printf("   Estimated wait: %d minutes\n", newReservation->estimatedWaitTime);
    
    return newReservation;
}

// Function to process next reservation
Reservation* processNextReservation(ReservationQueue* queue, int counterId) {
    if (isReservationQueueEmpty(queue)) {
        printf("❌ No reservations in queue to process!\n");
        return NULL;
    }
    
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return NULL;
    }
    
    Counter* counter = &queue->counters[counterId - 1];
    if (!counter->isActive) {
        printf("❌ Counter %d is not active!\n", counterId);
        return NULL;
    }
    
    if (counter->currentReservation != NULL) {
        printf("❌ Counter %d is currently processing reservation %d!\n", 
               counterId, counter->currentReservation->reservationId);
        return NULL;
    }
    
    // Find appropriate reservation for this counter
    Reservation* reservation = NULL;
    Reservation* previous = NULL;
    Reservation* current = queue->front;
    
    // If counter has specialization, prioritize matching reservations
    if (counter->specialization != 0) {
        while (current != NULL) {
            if (current->type == counter->specialization) {
                reservation = current;
                break;
            }
            previous = current;
            current = current->next;
        }
    }
    
    // If no specialized reservation found, take the first available
    if (reservation == NULL) {
        reservation = queue->front;
        previous = NULL;
    }
    
    // Remove reservation from queue
    if (previous == NULL) {
        queue->front = reservation->next;
        if (queue->front == NULL) {
            queue->rear = NULL;
        }
    } else {
        previous->next = reservation->next;
        if (reservation == queue->rear) {
            queue->rear = previous;
        }
    }
    
    queue->size--;
    reservation->next = NULL;
    reservation->processTime = time(NULL);
    
    // Assign to counter
    counter->currentReservation = reservation;
    
    printf("🎟️  Processing reservation at Counter %d (%s)\n", counterId, counter->counterName);
    printf("   Reservation ID: %d\n", reservation->reservationId);
    printf("   Customer: %s\n", reservation->customerName);
    printf("   Operator: %s\n", counter->operatorName);
    
    return reservation;
}

// Function to confirm reservation
void confirmReservation(ReservationQueue* queue, int counterId) {
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return;
    }
    
    Counter* counter = &queue->counters[counterId - 1];
    
    if (counter->currentReservation == NULL) {
        printf("❌ No reservation being processed at counter %d!\n", counterId);
        return;
    }
    
    Reservation* reservation = counter->currentReservation;
    reservation->status = TICKET_CONFIRMED;
    reservation->confirmationTime = time(NULL);
    
    // Update statistics
    queue->totalConfirmed++;
    queue->totalRevenue += reservation->totalAmount;
    counter->totalProcessed++;
    
    // Update average processing time
    float processingTime = difftime(reservation->confirmationTime, reservation->processTime) / 60.0f;
    counter->avgProcessingTime = ((counter->avgProcessingTime * (counter->totalProcessed - 1)) + processingTime) / counter->totalProcessed;
    
    printf("✅ Reservation %d confirmed successfully!\n", reservation->reservationId);
    printf("   Customer: %s\n", reservation->customerName);
    printf("   Total Amount: $%.2f\n", reservation->totalAmount);
    printf("   Processing Time: %.1f minutes\n", processingTime);
    printf("   Total Wait Time: %.1f minutes\n", 
           difftime(reservation->confirmationTime, reservation->requestTime) / 60.0f);
    
    // Generate confirmation details
    printf("\n--- TICKET CONFIRMATION ---\n");
    printf("Confirmation Number: TKT%d\n", reservation->reservationId);
    printf("Customer: %s\n", reservation->customerName);
    printf("Email: %s\n", reservation->email);
    printf("Phone: %s\n", reservation->phoneNumber);
    printf("Event: %s\n", reservation->eventName);
    printf("Location: %s\n", reservation->location);
    printf("Date/Time: %s\n", reservation->dateTime);
    printf("Number of Tickets: %d\n", reservation->numberOfTickets);
    printf("Total Amount: $%.2f\n", reservation->totalAmount);
    printf("Status: CONFIRMED\n");
    printf("---------------------------\n");
    
    // Free the reservation and reset counter
    free(reservation);
    counter->currentReservation = NULL;
    
    printf("Counter %d is now available for next customer\n", counterId);
}

// Function to cancel reservation
void cancelReservation(ReservationQueue* queue, int counterId) {
    if (counterId < 1 || counterId > queue->counterCount) {
        printf("❌ Invalid counter ID!\n");
        return;
    }
    
    Counter* counter = &queue->counters[counterId - 1];
    
    if (counter->currentReservation == NULL) {
        printf("❌ No reservation being processed at counter %d!\n", counterId);
        return;
    }
    
    Reservation* reservation = counter->currentReservation;
    reservation->status = TICKET_CANCELLED;
    
    queue->totalCancelled++;
    counter->totalProcessed++;
    
    printf("❌ Reservation %d cancelled\n", reservation->reservationId);
    printf("   Customer: %s\n", reservation->customerName);
    printf("   Reason: Customer request / Payment issue\n");
    
    // Free the reservation and reset counter
    free(reservation);
    counter->currentReservation = NULL;
    
    printf("Counter %d is now available for next customer\n", counterId);
}

// Function to display queue status
void displayQueueStatus(ReservationQueue* queue) {
    printf("\n=== RESERVATION QUEUE STATUS ===\n");
    printf("Reservations in queue: %d\n", queue->size);
    printf("Next reservation ID: %d\n", queue->nextReservationId);
    printf("Total reservations: %d\n", queue->totalReservations);
    printf("Total confirmed: %d\n", queue->totalConfirmed);
    printf("Total cancelled: %d\n", queue->totalCancelled);
    printf("Total revenue: $%.2f\n", queue->totalRevenue);
    printf("Success rate: %.1f%%\n", 
           queue->totalReservations > 0 ? (float)queue->totalConfirmed / queue->totalReservations * 100 : 0);
    
    if (isReservationQueueEmpty(queue)) {
        printf("\nNo reservations waiting in queue\n");
    } else {
        printf("\nWaiting reservations:\n");
        printf("%-6s %-20s %-8s %-25s %-8s %-10s %-8s\n", 
               "ID", "Customer", "Type", "Event", "Tickets", "Amount", "Priority");
        printf("-------------------------------------------------------------------------------------\n");
        
        Reservation* current = queue->front;
        while (current != NULL) {
            printf("%-6d %-20s %-8s %-25s %-8d $%-9.2f %-8s\n",
                   current->reservationId,
                   current->customerName,
                   getReservationTypeString(current->type),
                   current->eventName,
                   current->numberOfTickets,
                   current->totalAmount,
                   getPriorityString(current->priority));
            current = current->next;
        }
    }
    printf("================================\n\n");
}

// Function to display counter status
void displayCounterStatus(ReservationQueue* queue) {
    if (queue->counterCount == 0) {
        printf("No service counters configured!\n");
        return;
    }
    
    printf("\n=== COUNTER STATUS ===\n");
    printf("%-8s %-20s %-15s %-10s %-15s %-12s %-10s\n",
           "Counter", "Name", "Operator", "Status", "Specialization", "Avg Time", "Processed");
    printf("------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < queue->counterCount; i++) {
        Counter* counter = &queue->counters[i];
        char status[20], specialization[20];
        
        if (!counter->isActive) {
            strcpy(status, "Inactive");
        } else if (counter->currentReservation == NULL) {
            strcpy(status, "Available");
        } else {
            sprintf(status, "Busy (ID:%d)", counter->currentReservation->reservationId);
        }
        
        if (counter->specialization == 0) {
            strcpy(specialization, "All Types");
        } else {
            strcpy(specialization, getReservationTypeString(counter->specialization));
        }
        
        printf("%-8d %-20s %-15s %-10s %-15s %-12.1f %-10d\n",
               counter->counterId,
               counter->counterName,
               counter->operatorName,
               status,
               specialization,
               counter->avgProcessingTime,
               counter->totalProcessed);
    }
    printf("=====================\n\n");
}

// Function to search reservation by ID
void searchReservation(ReservationQueue* queue, int reservationId) {
    // Check in queue
    Reservation* current = queue->front;
    while (current != NULL) {
        if (current->reservationId == reservationId) {
            printf("\n=== RESERVATION FOUND (In Queue) ===\n");
            printf("Reservation ID: %d\n", current->reservationId);
            printf("Customer: %s\n", current->customerName);
            printf("Email: %s\n", current->email);
            printf("Phone: %s\n", current->phoneNumber);
            printf("Type: %s\n", getReservationTypeString(current->type));
            printf("Event: %s\n", current->eventName);
            printf("Location: %s\n", current->location);
            printf("Date/Time: %s\n", current->dateTime);
            printf("Tickets: %d\n", current->numberOfTickets);
            printf("Amount: $%.2f\n", current->totalAmount);
            printf("Priority: %s\n", getPriorityString(current->priority));
            printf("Status: %s\n", getTicketStatusString(current->status));
            printf("Wait Time: %d minutes\n", current->estimatedWaitTime);
            printf("===================================\n");
            return;
        }
        current = current->next;
    }
    
    // Check at counters
    for (int i = 0; i < queue->counterCount; i++) {
        if (queue->counters[i].currentReservation != NULL &&
            queue->counters[i].currentReservation->reservationId == reservationId) {
            
            Reservation* res = queue->counters[i].currentReservation;
            printf("\n=== RESERVATION FOUND (Being Processed) ===\n");
            printf("Reservation ID: %d\n", res->reservationId);
            printf("Customer: %s\n", res->customerName);
            printf("Current Status: Being processed at Counter %d\n", queue->counters[i].counterId);
            printf("Operator: %s\n", queue->counters[i].operatorName);
            printf("Processing Time: %.1f minutes\n", 
                   difftime(time(NULL), res->processTime) / 60.0f);
            printf("==========================================\n");
            return;
        }
    }
    
    printf("❌ Reservation ID %d not found!\n", reservationId);
}

// Function to setup sample data
void setupSampleReservationData(ReservationQueue* queue) {
    printf("Setting up sample reservation data...\n");
    
    // Add service counters
    addCounter(queue, "Movie Tickets", "Alice Johnson", RESERVATION_MOVIE);
    addCounter(queue, "Flight Booking", "Bob Smith", RESERVATION_FLIGHT);
    addCounter(queue, "Train Reservations", "Carol Davis", RESERVATION_TRAIN);
    addCounter(queue, "Event Tickets", "David Wilson", RESERVATION_EVENT);
    addCounter(queue, "General Service", "Emma Brown", 0); // All types
    
    // Add sample reservations
    addReservation(queue, "John Doe", "john.doe@email.com", "555-0101",
                  RESERVATION_MOVIE, "Avengers: Endgame", "Cinema City Mall", 
                  "2024-01-15 19:30", 2, 24.50f, PRIORITY_NORMAL);
    
    addReservation(queue, "Sarah Johnson", "sarah.j@email.com", "555-0102",
                  RESERVATION_FLIGHT, "Flight AA123", "New York to London", 
                  "2024-01-20 14:30", 1, 750.00f, PRIORITY_PREMIUM);
    
    addReservation(queue, "Mike Chen", "mike.chen@email.com", "555-0103",
                  RESERVATION_TRAIN, "Express Train 456", "Boston to Washington", 
                  "2024-01-18 08:15", 2, 180.00f, PRIORITY_NORMAL);
    
    addReservation(queue, "Lisa Williams", "lisa.w@email.com", "555-0104",
                  RESERVATION_EVENT, "Concert: Rock Legends", "Madison Square Garden", 
                  "2024-01-25 20:00", 4, 320.00f, PRIORITY_VIP);
    
    addReservation(queue, "Emergency Travel", "urgent@email.com", "555-0105",
                  RESERVATION_FLIGHT, "Emergency Flight", "Medical Emergency", 
                  "2024-01-16 ASAP", 1, 1200.00f, PRIORITY_EMERGENCY);
    
    addReservation(queue, "Robert Garcia", "rob.garcia@email.com", "555-0106",
                  RESERVATION_HOTEL, "Grand Hotel Suite", "Downtown Manhattan", 
                  "2024-01-22 Check-in", 1, 450.00f, PRIORITY_NORMAL);
    
    printf("Sample reservation data setup complete!\n");
}

// Function to free the reservation queue
void freeReservationQueue(ReservationQueue* queue) {
    while (!isReservationQueueEmpty(queue)) {
        Reservation* reservation = queue->front;
        queue->front = queue->front->next;
        free(reservation);
    }
    
    // Free any reservations still being processed
    for (int i = 0; i < queue->counterCount; i++) {
        if (queue->counters[i].currentReservation != NULL) {
            free(queue->counters[i].currentReservation);
        }
    }
    
    free(queue);
}

// Main function with menu-driven interface
int main() {
    ReservationQueue* reservationQueue = createReservationQueue();
    int choice, counterId, reservationType, numTickets, priorityLevel, reservationId;
    float amount;
    char name[MAX_NAME_LENGTH], email[MAX_NAME_LENGTH], phone[20];
    char eventName[MAX_EVENT_NAME], location[MAX_LOCATION_NAME], dateTime[30];
    char counterName[MAX_NAME_LENGTH], operatorName[MAX_NAME_LENGTH];
    
    printf("🎫 Welcome to Reservation & Ticket Counter System! ✈️\n");
    printf("Efficient queue management for all your booking needs!\n\n");
    
    do {
        printf("\n=== RESERVATION MANAGEMENT SYSTEM ===\n");
        printf("1. Create new reservation\n");
        printf("2. Process next reservation\n");
        printf("3. Confirm reservation\n");
        printf("4. Cancel reservation\n");
        printf("5. Display queue status\n");
        printf("6. Display counter status\n");
        printf("7. Search reservation by ID\n");
        printf("8. Add service counter\n");
        printf("9. Setup sample data\n");
        printf("10. Generate daily report\n");
        printf("11. Clear all data\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter customer name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                
                printf("Enter phone number: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = 0;
                
                printf("Reservation types: 1-Movie, 2-Flight, 3-Train, 4-Event, 5-Hotel\n");
                printf("Enter reservation type (1-5): ");
                scanf("%d", &reservationType);
                getchar();
                
                printf("Enter event/service name: ");
                fgets(eventName, sizeof(eventName), stdin);
                eventName[strcspn(eventName, "\n")] = 0;
                
                printf("Enter location: ");
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;
                
                printf("Enter date/time: ");
                fgets(dateTime, sizeof(dateTime), stdin);
                dateTime[strcspn(dateTime, "\n")] = 0;
                
                printf("Enter number of tickets: ");
                scanf("%d", &numTickets);
                
                printf("Enter total amount: $");
                scanf("%f", &amount);
                
                printf("Priority levels: 1-Normal, 2-Premium, 3-VIP, 4-Emergency\n");
                printf("Enter priority level (1-4): ");
                scanf("%d", &priorityLevel);
                
                if (reservationType >= 1 && reservationType <= 5 && 
                    priorityLevel >= 1 && priorityLevel <= 4) {
                    addReservation(reservationQueue, name, email, phone,
                                 (ReservationType)reservationType, eventName, location, dateTime,
                                 numTickets, amount, (PriorityLevel)priorityLevel);
                } else {
                    printf("Invalid reservation type or priority level!\n");
                }
                break;
                
            case 2:
                printf("Enter counter ID to process next reservation: ");
                scanf("%d", &counterId);
                processNextReservation(reservationQueue, counterId);
                break;
                
            case 3:
                printf("Enter counter ID to confirm reservation: ");
                scanf("%d", &counterId);
                confirmReservation(reservationQueue, counterId);
                break;
                
            case 4:
                printf("Enter counter ID to cancel reservation: ");
                scanf("%d", &counterId);
                cancelReservation(reservationQueue, counterId);
                break;
                
            case 5:
                displayQueueStatus(reservationQueue);
                break;
                
            case 6:
                displayCounterStatus(reservationQueue);
                break;
                
            case 7:
                printf("Enter reservation ID to search: ");
                scanf("%d", &reservationId);
                searchReservation(reservationQueue, reservationId);
                break;
                
            case 8:
                printf("Enter counter name: ");
                fgets(counterName, sizeof(counterName), stdin);
                counterName[strcspn(counterName, "\n")] = 0;
                
                printf("Enter operator name: ");
                fgets(operatorName, sizeof(operatorName), stdin);
                operatorName[strcspn(operatorName, "\n")] = 0;
                
                printf("Specialization (0-All, 1-Movie, 2-Flight, 3-Train, 4-Event, 5-Hotel): ");
                scanf("%d", &reservationType);
                
                if (reservationType >= 0 && reservationType <= 5) {
                    addCounter(reservationQueue, counterName, operatorName, 
                             (ReservationType)reservationType);
                } else {
                    printf("Invalid specialization!\n");
                }
                break;
                
            case 9:
                setupSampleReservationData(reservationQueue);
                break;
                
            case 10:
                printf("\n=== DAILY REPORT ===\n");
                printf("Total Reservations: %d\n", reservationQueue->totalReservations);
                printf("Confirmed Reservations: %d\n", reservationQueue->totalConfirmed);
                printf("Cancelled Reservations: %d\n", reservationQueue->totalCancelled);
                printf("Pending Reservations: %d\n", reservationQueue->size);
                printf("Total Revenue: $%.2f\n", reservationQueue->totalRevenue);
                printf("Success Rate: %.1f%%\n", 
                       reservationQueue->totalReservations > 0 ? 
                       (float)reservationQueue->totalConfirmed / reservationQueue->totalReservations * 100 : 0);
                printf("==================\n");
                break;
                
            case 11:
                freeReservationQueue(reservationQueue);
                reservationQueue = createReservationQueue();
                printf("All data cleared!\n");
                break;
                
            case 12:
                printf("Thanks for using Reservation Management System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 12);
    
    freeReservationQueue(reservationQueue);
    return 0;
}