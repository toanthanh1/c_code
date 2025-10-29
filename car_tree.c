/*
 * Car Manufacturer and Type Identification Tree
 * This program implements a binary search tree to identify car manufacturer
 * and car type by providing the car name/model.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a car in the tree
typedef struct CarNode {
    char carName[50];
    char manufacturer[50];
    char carType[30];
    int year;
    char engineType[20];
    struct CarNode* left;
    struct CarNode* right;
} CarNode;

// Structure to represent the car database tree
typedef struct CarTree {
    CarNode* root;
    int totalCars;
} CarTree;

// Function to create a new car node
CarNode* createCarNode(char* name, char* manufacturer, char* type, int year, char* engine) {
    CarNode* newNode = (CarNode*)malloc(sizeof(CarNode));
    strcpy(newNode->carName, name);
    strcpy(newNode->manufacturer, manufacturer);
    strcpy(newNode->carType, type);
    newNode->year = year;
    strcpy(newNode->engineType, engine);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to create the car tree
CarTree* createCarTree() {
    CarTree* tree = (CarTree*)malloc(sizeof(CarTree));
    tree->root = NULL;
    tree->totalCars = 0;
    return tree;
}

// Function to insert a car into the tree (BST based on car name)
CarNode* insertCar(CarNode* root, char* name, char* manufacturer, char* type, int year, char* engine) {
    if (root == NULL) {
        return createCarNode(name, manufacturer, type, year, engine);
    }
    
    int cmp = strcmp(name, root->carName);
    
    if (cmp < 0) {
        root->left = insertCar(root->left, name, manufacturer, type, year, engine);
    } else if (cmp > 0) {
        root->right = insertCar(root->right, name, manufacturer, type, year, engine);
    } else {
        // Car already exists - update information
        strcpy(root->manufacturer, manufacturer);
        strcpy(root->carType, type);
        root->year = year;
        strcpy(root->engineType, engine);
        printf("Updated information for %s\n", name);
        return root;
    }
    
    return root;
}

// Function to add a car to the tree
void addCar(CarTree* tree, char* name, char* manufacturer, char* type, int year, char* engine) {
    CarNode* existingCar = searchCar(tree->root, name);
    if (existingCar == NULL) {
        tree->totalCars++;
        printf("Added: %s (%s %s)\n", name, manufacturer, type);
    }
    tree->root = insertCar(tree->root, name, manufacturer, type, year, engine);
}

// Function to search for a car by name
CarNode* searchCar(CarNode* root, char* name) {
    if (root == NULL) {
        return NULL;
    }
    
    int cmp = strcmp(name, root->carName);
    
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return searchCar(root->left, name);
    } else {
        return searchCar(root->right, name);
    }
}

// Function to display car information
void displayCarInfo(CarNode* car) {
    if (car == NULL) {
        printf("Car not found!\n");
        return;
    }
    
    printf("\n=== CAR INFORMATION ===\n");
    printf("Car Name: %s\n", car->carName);
    printf("Manufacturer: %s\n", car->manufacturer);
    printf("Car Type: %s\n", car->carType);
    printf("Year: %d\n", car->year);
    printf("Engine Type: %s\n", car->engineType);
    printf("======================\n\n");
}

// Function to find cars by manufacturer
void findCarsByManufacturer(CarNode* root, char* manufacturer) {
    if (root == NULL) {
        return;
    }
    
    if (strcmp(root->manufacturer, manufacturer) == 0) {
        printf("- %s (%s, %d)\n", root->carName, root->carType, root->year);
    }
    
    findCarsByManufacturer(root->left, manufacturer);
    findCarsByManufacturer(root->right, manufacturer);
}

// Function to find cars by type
void findCarsByType(CarNode* root, char* type) {
    if (root == NULL) {
        return;
    }
    
    if (strcmp(root->carType, type) == 0) {
        printf("- %s by %s (%d)\n", root->carName, root->manufacturer, root->year);
    }
    
    findCarsByType(root->left, type);
    findCarsByType(root->right, type);
}

// Function to display all cars in alphabetical order (in-order traversal)
void displayAllCars(CarNode* root) {
    if (root == NULL) {
        return;
    }
    
    displayAllCars(root->left);
    printf("%-20s %-15s %-12s %d %-10s\n", 
           root->carName, root->manufacturer, root->carType, 
           root->year, root->engineType);
    displayAllCars(root->right);
}

// Function to count cars by manufacturer
int countCarsByManufacturer(CarNode* root, char* manufacturer) {
    if (root == NULL) {
        return 0;
    }
    
    int count = 0;
    if (strcmp(root->manufacturer, manufacturer) == 0) {
        count = 1;
    }
    
    return count + countCarsByManufacturer(root->left, manufacturer) + 
           countCarsByManufacturer(root->right, manufacturer);
}

// Function to find the minimum car node (leftmost)
CarNode* findMin(CarNode* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Function to delete a car from the tree
CarNode* deleteCar(CarNode* root, char* name) {
    if (root == NULL) {
        return root;
    }
    
    int cmp = strcmp(name, root->carName);
    
    if (cmp < 0) {
        root->left = deleteCar(root->left, name);
    } else if (cmp > 0) {
        root->right = deleteCar(root->right, name);
    } else {
        // Found the node to delete
        if (root->left == NULL) {
            CarNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            CarNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children
        CarNode* temp = findMin(root->right);
        
        // Copy the inorder successor's data to this node
        strcpy(root->carName, temp->carName);
        strcpy(root->manufacturer, temp->manufacturer);
        strcpy(root->carType, temp->carType);
        root->year = temp->year;
        strcpy(root->engineType, temp->engineType);
        
        // Delete the inorder successor
        root->right = deleteCar(root->right, temp->carName);
    }
    
    return root;
}

// Function to get tree height
int getTreeHeight(CarNode* root) {
    if (root == NULL) {
        return 0;
    }
    
    int leftHeight = getTreeHeight(root->left);
    int rightHeight = getTreeHeight(root->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Function to display tree statistics
void displayTreeStats(CarTree* tree) {
    printf("\n=== TREE STATISTICS ===\n");
    printf("Total cars in database: %d\n", tree->totalCars);
    printf("Tree height: %d\n", getTreeHeight(tree->root));
    
    // Count cars by some manufacturers
    printf("\nCars by manufacturer:\n");
    printf("Toyota: %d\n", countCarsByManufacturer(tree->root, "Toyota"));
    printf("Honda: %d\n", countCarsByManufacturer(tree->root, "Honda"));
    printf("Ford: %d\n", countCarsByManufacturer(tree->root, "Ford"));
    printf("BMW: %d\n", countCarsByManufacturer(tree->root, "BMW"));
    printf("Mercedes: %d\n", countCarsByManufacturer(tree->root, "Mercedes"));
    printf("=====================\n\n");
}

// Function to populate sample car data
void populateSampleData(CarTree* tree) {
    printf("Loading sample car database...\n\n");
    
    // Toyota cars
    addCar(tree, "Camry", "Toyota", "Sedan", 2023, "Hybrid");
    addCar(tree, "Corolla", "Toyota", "Sedan", 2023, "Gasoline");
    addCar(tree, "Prius", "Toyota", "Hybrid", 2023, "Hybrid");
    addCar(tree, "RAV4", "Toyota", "SUV", 2023, "Gasoline");
    addCar(tree, "Highlander", "Toyota", "SUV", 2023, "Hybrid");
    
    // Honda cars
    addCar(tree, "Civic", "Honda", "Sedan", 2023, "Gasoline");
    addCar(tree, "Accord", "Honda", "Sedan", 2023, "Hybrid");
    addCar(tree, "CR-V", "Honda", "SUV", 2023, "Gasoline");
    addCar(tree, "Pilot", "Honda", "SUV", 2023, "Gasoline");
    addCar(tree, "Insight", "Honda", "Hybrid", 2023, "Hybrid");
    
    // Ford cars
    addCar(tree, "F-150", "Ford", "Truck", 2023, "Gasoline");
    addCar(tree, "Mustang", "Ford", "Sports", 2023, "Gasoline");
    addCar(tree, "Explorer", "Ford", "SUV", 2023, "Gasoline");
    addCar(tree, "Escape", "Ford", "SUV", 2023, "Hybrid");
    addCar(tree, "Fusion", "Ford", "Sedan", 2020, "Hybrid");
    
    // BMW cars
    addCar(tree, "3 Series", "BMW", "Sedan", 2023, "Gasoline");
    addCar(tree, "5 Series", "BMW", "Sedan", 2023, "Gasoline");
    addCar(tree, "X3", "BMW", "SUV", 2023, "Gasoline");
    addCar(tree, "X5", "BMW", "SUV", 2023, "Gasoline");
    addCar(tree, "i4", "BMW", "Electric", 2023, "Electric");
    
    // Mercedes cars
    addCar(tree, "C-Class", "Mercedes", "Sedan", 2023, "Gasoline");
    addCar(tree, "E-Class", "Mercedes", "Sedan", 2023, "Gasoline");
    addCar(tree, "GLC", "Mercedes", "SUV", 2023, "Gasoline");
    addCar(tree, "GLE", "Mercedes", "SUV", 2023, "Gasoline");
    addCar(tree, "EQS", "Mercedes", "Electric", 2023, "Electric");
    
    printf("Sample data loaded successfully!\n");
}

// Function to free the tree memory
void freeCarTree(CarNode* root) {
    if (root == NULL) {
        return;
    }
    
    freeCarTree(root->left);
    freeCarTree(root->right);
    free(root);
}

// Main function with menu-driven interface
int main() {
    CarTree* carTree = createCarTree();
    int choice, year;
    char name[50], manufacturer[50], type[30], engine[20];
    CarNode* foundCar;
    
    printf("🚗 Welcome to Car Identification Database! 🚗\n");
    printf("Find car manufacturer and type information easily!\n\n");
    
    do {
        printf("\n=== CAR DATABASE MENU ===\n");
        printf("1. Add car\n");
        printf("2. Search car by name\n");
        printf("3. Find cars by manufacturer\n");
        printf("4. Find cars by type\n");
        printf("5. Display all cars\n");
        printf("6. Delete car\n");
        printf("7. Display tree statistics\n");
        printf("8. Load sample data\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter car name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                printf("Enter manufacturer: ");
                fgets(manufacturer, sizeof(manufacturer), stdin);
                manufacturer[strcspn(manufacturer, "\n")] = 0;
                
                printf("Enter car type (Sedan/SUV/Truck/Sports/Hybrid/Electric): ");
                fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = 0;
                
                printf("Enter year: ");
                scanf("%d", &year);
                getchar();
                
                printf("Enter engine type (Gasoline/Hybrid/Electric/Diesel): ");
                fgets(engine, sizeof(engine), stdin);
                engine[strcspn(engine, "\n")] = 0;
                
                addCar(carTree, name, manufacturer, type, year, engine);
                break;
                
            case 2:
                printf("Enter car name to search: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                foundCar = searchCar(carTree->root, name);
                displayCarInfo(foundCar);
                break;
                
            case 3:
                printf("Enter manufacturer name: ");
                fgets(manufacturer, sizeof(manufacturer), stdin);
                manufacturer[strcspn(manufacturer, "\n")] = 0;
                
                printf("\nCars by %s:\n", manufacturer);
                findCarsByManufacturer(carTree->root, manufacturer);
                printf("\n");
                break;
                
            case 4:
                printf("Enter car type: ");
                fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = 0;
                
                printf("\n%s cars:\n", type);
                findCarsByType(carTree->root, type);
                printf("\n");
                break;
                
            case 5:
                if (carTree->totalCars == 0) {
                    printf("No cars in database!\n");
                } else {
                    printf("\n=== ALL CARS (Alphabetical Order) ===\n");
                    printf("%-20s %-15s %-12s %-6s %-10s\n", 
                           "Car Name", "Manufacturer", "Type", "Year", "Engine");
                    printf("----------------------------------------------------------------\n");
                    displayAllCars(carTree->root);
                    printf("\n");
                }
                break;
                
            case 6:
                printf("Enter car name to delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                foundCar = searchCar(carTree->root, name);
                if (foundCar != NULL) {
                    carTree->root = deleteCar(carTree->root, name);
                    carTree->totalCars--;
                    printf("Car '%s' deleted successfully!\n", name);
                } else {
                    printf("Car '%s' not found!\n", name);
                }
                break;
                
            case 7:
                displayTreeStats(carTree);
                break;
                
            case 8:
                populateSampleData(carTree);
                break;
                
            case 9:
                printf("Thanks for using Car Database!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 9);
    
    freeCarTree(carTree->root);
    free(carTree);
    return 0;
}