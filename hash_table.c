/*
 * Hash Table Implementation using Linked List (Chaining)
 * This program implements a hash table with collision resolution
 * using separate chaining with linked lists.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// Structure to represent a key-value pair in the hash table
typedef struct HashNode {
    char key[50];
    char value[100];
    struct HashNode* next;
} HashNode;

// Structure to represent the hash table
typedef struct HashTable {
    HashNode* table[TABLE_SIZE];
    int totalElements;
    int collisions;
} HashTable;

// Function to create a new hash node
HashNode* createHashNode(char* key, char* value) {
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, key);
    strcpy(newNode->value, value);
    newNode->next = NULL;
    return newNode;
}

// Function to create a hash table
HashTable* createHashTable() {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->totalElements = 0;
    hashTable->collisions = 0;
    
    // Initialize all buckets to NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    
    return hashTable;
}

// Simple hash function (sum of ASCII values % table size)
int hashFunction(char* key) {
    int hash = 0;
    int len = strlen(key);
    
    for (int i = 0; i < len; i++) {
        hash += key[i];
    }
    
    return hash % TABLE_SIZE;
}

// Better hash function (djb2 algorithm)
int hashFunctionDJB2(char* key) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % TABLE_SIZE;
}

// Function to insert a key-value pair into the hash table
void insert(HashTable* hashTable, char* key, char* value) {
    int index = hashFunction(key);
    HashNode* newNode = createHashNode(key, value);
    
    // Check if bucket is empty
    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newNode;
        printf("Inserted: %s -> %s at index %d\n", key, value, index);
    } else {
        // Collision occurred - check if key already exists
        HashNode* current = hashTable->table[index];
        HashNode* prev = NULL;
        
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                // Key already exists - update value
                strcpy(current->value, value);
                printf("Updated: %s -> %s at index %d\n", key, value, index);
                free(newNode); // Don't need the new node
                return;
            }
            prev = current;
            current = current->next;
        }
        
        // Add new node to the end of the chain
        prev->next = newNode;
        hashTable->collisions++;
        printf("Inserted: %s -> %s at index %d (collision resolved)\n", key, value, index);
    }
    
    hashTable->totalElements++;
}

// Function to search for a value by key
char* search(HashTable* hashTable, char* key) {
    int index = hashFunction(key);
    HashNode* current = hashTable->table[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL; // Key not found
}

// Function to delete a key-value pair
int deleteKey(HashTable* hashTable, char* key) {
    int index = hashFunction(key);
    HashNode* current = hashTable->table[index];
    HashNode* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Found the key to delete
            if (prev == NULL) {
                // Deleting the first node in the chain
                hashTable->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            
            printf("Deleted: %s from index %d\n", key, index);
            free(current);
            hashTable->totalElements--;
            return 1; // Success
        }
        prev = current;
        current = current->next;
    }
    
    return 0; // Key not found
}

// Function to display the entire hash table
void displayHashTable(HashTable* hashTable) {
    printf("\n=== HASH TABLE CONTENTS ===\n");
    printf("Total elements: %d\n", hashTable->totalElements);
    printf("Total collisions: %d\n", hashTable->collisions);
    printf("Load factor: %.2f\n\n", (float)hashTable->totalElements / TABLE_SIZE);
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket %d: ", i);
        
        if (hashTable->table[i] == NULL) {
            printf("[EMPTY]\n");
        } else {
            HashNode* current = hashTable->table[i];
            while (current != NULL) {
                printf("[%s: %s]", current->key, current->value);
                if (current->next != NULL) {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Function to display hash table statistics
void displayStatistics(HashTable* hashTable) {
    printf("\n=== HASH TABLE STATISTICS ===\n");
    printf("Table size: %d\n", TABLE_SIZE);
    printf("Total elements: %d\n", hashTable->totalElements);
    printf("Total collisions: %d\n", hashTable->collisions);
    printf("Load factor: %.2f\n", (float)hashTable->totalElements / TABLE_SIZE);
    
    // Calculate chain lengths
    int maxChainLength = 0;
    int emptyBuckets = 0;
    int totalChainLength = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        int chainLength = 0;
        HashNode* current = hashTable->table[i];
        
        if (current == NULL) {
            emptyBuckets++;
        } else {
            while (current != NULL) {
                chainLength++;
                current = current->next;
            }
            totalChainLength += chainLength;
            if (chainLength > maxChainLength) {
                maxChainLength = chainLength;
            }
        }
    }
    
    printf("Empty buckets: %d\n", emptyBuckets);
    printf("Non-empty buckets: %d\n", TABLE_SIZE - emptyBuckets);
    printf("Maximum chain length: %d\n", maxChainLength);
    if (TABLE_SIZE - emptyBuckets > 0) {
        printf("Average chain length: %.2f\n", 
               (float)totalChainLength / (TABLE_SIZE - emptyBuckets));
    }
    printf("\n");
}

// Function to rehash the table (resize and redistribute)
HashTable* rehash(HashTable* oldTable) {
    printf("Rehashing the table...\n");
    
    // Create new table (in a real implementation, you'd increase the size)
    HashTable* newTable = createHashTable();
    
    // Redistribute all elements
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = oldTable->table[i];
        while (current != NULL) {
            insert(newTable, current->key, current->value);
            current = current->next;
        }
    }
    
    // Free old table
    freeHashTable(oldTable);
    
    printf("Rehashing completed!\n");
    return newTable;
}

// Function to get all keys
void getAllKeys(HashTable* hashTable) {
    printf("\n=== ALL KEYS ===\n");
    int count = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = hashTable->table[i];
        while (current != NULL) {
            printf("%d. %s\n", ++count, current->key);
            current = current->next;
        }
    }
    
    if (count == 0) {
        printf("No keys found.\n");
    }
    printf("\n");
}

// Function to search by value (reverse lookup)
void searchByValue(HashTable* hashTable, char* value) {
    printf("\n=== SEARCHING FOR VALUE: %s ===\n", value);
    int found = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = hashTable->table[i];
        while (current != NULL) {
            if (strcmp(current->value, value) == 0) {
                printf("Found key: %s at bucket %d\n", current->key, i);
                found++;
            }
            current = current->next;
        }
    }
    
    if (found == 0) {
        printf("Value not found in hash table.\n");
    } else {
        printf("Total matches: %d\n", found);
    }
    printf("\n");
}

// Function to free the hash table memory
void freeHashTable(HashTable* hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = hashTable->table[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable);
}

// Function to populate with sample data
void populateSampleData(HashTable* hashTable) {
    insert(hashTable, "apple", "A red or green fruit");
    insert(hashTable, "banana", "A yellow tropical fruit");
    insert(hashTable, "orange", "A citrus fruit");
    insert(hashTable, "grape", "Small round fruit in bunches");
    insert(hashTable, "strawberry", "Red berry with seeds outside");
    insert(hashTable, "pineapple", "Tropical fruit with spiky skin");
    insert(hashTable, "mango", "Sweet tropical fruit");
    insert(hashTable, "kiwi", "Brown fuzzy fruit with green inside");
    insert(hashTable, "peach", "Fuzzy orange/pink fruit");
    insert(hashTable, "watermelon", "Large green fruit with red inside");
    insert(hashTable, "cherry", "Small red fruit with pit");
    insert(hashTable, "blueberry", "Small blue antioxidant-rich fruit");
    
    printf("\nSample data loaded!\n");
}

// Main function with menu-driven interface
int main() {
    HashTable* hashTable = createHashTable();
    int choice;
    char key[50], value[100], searchValue[100];
    char* result;
    
    printf("🔢 Welcome to Hash Table with Chaining! 🔢\n");
    printf("Efficient key-value storage with collision resolution!\n\n");
    
    do {
        printf("\n=== HASH TABLE MENU ===\n");
        printf("1. Insert key-value pair\n");
        printf("2. Search by key\n");
        printf("3. Delete key\n");
        printf("4. Display hash table\n");
        printf("5. Display statistics\n");
        printf("6. Get all keys\n");
        printf("7. Search by value\n");
        printf("8. Load sample data\n");
        printf("9. Rehash table\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter key: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                
                printf("Enter value: ");
                fgets(value, sizeof(value), stdin);
                value[strcspn(value, "\n")] = 0;
                
                insert(hashTable, key, value);
                break;
                
            case 2:
                printf("Enter key to search: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                
                result = search(hashTable, key);
                if (result != NULL) {
                    printf("Found: %s -> %s\n", key, result);
                } else {
                    printf("Key '%s' not found!\n", key);
                }
                break;
                
            case 3:
                printf("Enter key to delete: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                
                if (deleteKey(hashTable, key)) {
                    printf("Key '%s' deleted successfully!\n", key);
                } else {
                    printf("Key '%s' not found!\n", key);
                }
                break;
                
            case 4:
                displayHashTable(hashTable);
                break;
                
            case 5:
                displayStatistics(hashTable);
                break;
                
            case 6:
                getAllKeys(hashTable);
                break;
                
            case 7:
                printf("Enter value to search: ");
                fgets(searchValue, sizeof(searchValue), stdin);
                searchValue[strcspn(searchValue, "\n")] = 0;
                
                searchByValue(hashTable, searchValue);
                break;
                
            case 8:
                populateSampleData(hashTable);
                break;
                
            case 9:
                hashTable = rehash(hashTable);
                break;
                
            case 10:
                printf("Thanks for using Hash Table!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 10);
    
    freeHashTable(hashTable);
    return 0;
}