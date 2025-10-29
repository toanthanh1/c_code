/*
 * Scavenger Hunt / Treasure Hunt Game using Linked List
 * This program implements a treasure hunt game where players follow clues
 * through different locations to find the final treasure using linked list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a location/clue in the hunt
typedef struct Location {
    int locationId;
    char locationName[100];
    char clue[200];
    char requiredItem[50];
    char treasureFound[50];
    int isVisited;
    int isFinalTreasure;
    struct Location* nextClue;
} Location;

// Structure to represent a player
typedef struct Player {
    char name[50];
    int playerId;
    Location* currentLocation;
    char inventory[10][50]; // Player's collected items
    int itemCount;
    int score;
    struct Player* next;
} Player;

// Structure to represent the game
typedef struct TreasureHunt {
    Location* startLocation;
    Player* players;
    int totalLocations;
    int totalPlayers;
} TreasureHunt;

// Function to create a new location
Location* createLocation(int id, char* name, char* clue, char* requiredItem, 
                        char* treasure, int isFinal) {
    Location* newLocation = (Location*)malloc(sizeof(Location));
    newLocation->locationId = id;
    strcpy(newLocation->locationName, name);
    strcpy(newLocation->clue, clue);
    strcpy(newLocation->requiredItem, requiredItem);
    strcpy(newLocation->treasureFound, treasure);
    newLocation->isVisited = 0;
    newLocation->isFinalTreasure = isFinal;
    newLocation->nextClue = NULL;
    return newLocation;
}

// Function to create a new player
Player* createPlayer(char* name, int id) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    strcpy(newPlayer->name, name);
    newPlayer->playerId = id;
    newPlayer->currentLocation = NULL;
    newPlayer->itemCount = 0;
    newPlayer->score = 0;
    newPlayer->next = NULL;
    return newPlayer;
}

// Function to initialize the treasure hunt game
TreasureHunt* createTreasureHunt() {
    TreasureHunt* game = (TreasureHunt*)malloc(sizeof(TreasureHunt));
    game->startLocation = NULL;
    game->players = NULL;
    game->totalLocations = 0;
    game->totalPlayers = 0;
    return game;
}

// Function to setup the treasure hunt locations
void setupHunt(TreasureHunt* game) {
    // Create locations with clues
    Location* park = createLocation(1, "Central Park", 
        "Look for the place where books are kept and knowledge is shared. Find the red door.", 
        "none", "Map Fragment", 0);
    
    Location* library = createLocation(2, "City Library", 
        "Where coffee flows and students study. Look for the blue table near the window.", 
        "Map Fragment", "Ancient Key", 0);
    
    Location* cafe = createLocation(3, "Campus Cafe", 
        "A place of learning with tall towers. The key opens the door to knowledge.", 
        "Ancient Key", "Golden Compass", 0);
    
    Location* university = createLocation(4, "University Tower", 
        "Where history sleeps and artifacts rest. The compass points to the past.", 
        "Golden Compass", "Crystal Orb", 0);
    
    Location* museum = createLocation(5, "History Museum", 
        "The final resting place where nature meets the sky. The orb reveals all.", 
        "Crystal Orb", "FINAL TREASURE: Golden Crown", 1);
    
    // Link the locations to create the hunt path
    park->nextClue = library;
    library->nextClue = cafe;
    cafe->nextClue = university;
    university->nextClue = museum;
    museum->nextClue = NULL; // End of hunt
    
    game->startLocation = park;
    game->totalLocations = 5;
    
    printf("Treasure hunt setup complete!\n");
    printf("Hunt path: Park -> Library -> Cafe -> University -> Museum\n\n");
}

// Function to add a player to the game
void addPlayer(TreasureHunt* game, char* name, int id) {
    Player* newPlayer = createPlayer(name, id);
    newPlayer->currentLocation = game->startLocation;
    
    if (game->players == NULL) {
        game->players = newPlayer;
    } else {
        Player* current = game->players;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPlayer;
    }
    
    game->totalPlayers++;
    printf("Player %s (ID: %d) joined the treasure hunt!\n", name, id);
}

// Function to add item to player's inventory
void addToInventory(Player* player, char* item) {
    if (player->itemCount < 10) {
        strcpy(player->inventory[player->itemCount], item);
        player->itemCount++;
        player->score += 10;
        printf("%s found: %s\n", player->name, item);
    } else {
        printf("Inventory full! Cannot carry more items.\n");
    }
}

// Function to check if player has required item
int hasRequiredItem(Player* player, char* requiredItem) {
    if (strcmp(requiredItem, "none") == 0) {
        return 1; // No item required
    }
    
    for (int i = 0; i < player->itemCount; i++) {
        if (strcmp(player->inventory[i], requiredItem) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function for player to explore current location
void exploreLocation(Player* player) {
    if (player->currentLocation == NULL) {
        printf("Player has completed the hunt!\n");
        return;
    }
    
    Location* loc = player->currentLocation;
    
    printf("\n=== %s is at: %s ===\n", player->name, loc->locationName);
    printf("Clue: %s\n", loc->clue);
    
    if (strcmp(loc->requiredItem, "none") != 0) {
        printf("Required item: %s\n", loc->requiredItem);
        
        if (!hasRequiredItem(player, loc->requiredItem)) {
            printf("You don't have the required item to proceed!\n");
            printf("Current inventory: ");
            for (int i = 0; i < player->itemCount; i++) {
                printf("%s ", player->inventory[i]);
            }
            printf("\n");
            return;
        }
    }
    
    // Player can explore this location
    if (!loc->isVisited) {
        loc->isVisited = 1;
        addToInventory(player, loc->treasureFound);
        player->score += 25; // Bonus for visiting new location
        
        if (loc->isFinalTreasure) {
            printf("\n🎉 CONGRATULATIONS! %s found the final treasure! 🎉\n", player->name);
            printf("Final treasure: %s\n", loc->treasureFound);
            player->score += 100; // Big bonus for completing hunt
            player->currentLocation = NULL; // Hunt completed
            return;
        }
    } else {
        printf("This location has already been explored.\n");
    }
}

// Function to move player to next location
void moveToNextLocation(Player* player) {
    if (player->currentLocation == NULL) {
        printf("Hunt already completed!\n");
        return;
    }
    
    Location* current = player->currentLocation;
    
    if (!hasRequiredItem(player, current->requiredItem)) {
        printf("Cannot move forward without required item: %s\n", current->requiredItem);
        return;
    }
    
    if (current->nextClue != NULL) {
        player->currentLocation = current->nextClue;
        printf("%s moved to: %s\n", player->name, player->currentLocation->locationName);
    } else {
        printf("This is the final location!\n");
    }
}

// Function to display player's inventory
void showInventory(Player* player) {
    printf("\n=== %s's Inventory ===\n", player->name);
    printf("Items collected: ");
    for (int i = 0; i < player->itemCount; i++) {
        printf("%s", player->inventory[i]);
        if (i < player->itemCount - 1) printf(", ");
    }
    printf("\nTotal items: %d\n", player->itemCount);
    printf("Current score: %d\n\n", player->score);
}

// Function to find player by ID
Player* findPlayer(TreasureHunt* game, int playerId) {
    Player* current = game->players;
    while (current != NULL) {
        if (current->playerId == playerId) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to display all players' status
void showAllPlayers(TreasureHunt* game) {
    if (game->players == NULL) {
        printf("No players in the game!\n");
        return;
    }
    
    printf("\n=== PLAYER STATUS ===\n");
    Player* current = game->players;
    while (current != NULL) {
        printf("Player: %s (ID: %d)\n", current->name, current->playerId);
        if (current->currentLocation != NULL) {
            printf("Current location: %s\n", current->currentLocation->locationName);
        } else {
            printf("Status: HUNT COMPLETED! 🏆\n");
        }
        printf("Score: %d\n", current->score);
        printf("Items: %d\n", current->itemCount);
        printf("-------------------\n");
        current = current->next;
    }
}

// Function to display game map/progress
void showGameMap(TreasureHunt* game) {
    printf("\n=== TREASURE HUNT MAP ===\n");
    Location* current = game->startLocation;
    int step = 1;
    
    while (current != NULL) {
        printf("%d. %s", step, current->locationName);
        if (current->isVisited) {
            printf(" ✓ (Explored)");
        }
        if (current->isFinalTreasure) {
            printf(" 🏆 (Final Treasure)");
        }
        printf("\n   Treasure: %s\n", current->treasureFound);
        if (current->nextClue != NULL) {
            printf("   |\n   v\n");
        }
        current = current->nextClue;
        step++;
    }
    printf("\n");
}

// Function to free game memory
void freeGame(TreasureHunt* game) {
    // Free locations
    Location* locCurrent = game->startLocation;
    while (locCurrent != NULL) {
        Location* locNext = locCurrent->nextClue;
        free(locCurrent);
        locCurrent = locNext;
    }
    
    // Free players
    Player* playerCurrent = game->players;
    while (playerCurrent != NULL) {
        Player* playerNext = playerCurrent->next;
        free(playerCurrent);
        playerCurrent = playerNext;
    }
    
    free(game);
}

// Main function with menu-driven interface
int main() {
    TreasureHunt* game = createTreasureHunt();
    int choice, playerId;
    char playerName[50];
    Player* currentPlayer;
    
    printf("🗺️  Welcome to the Treasure Hunt Adventure! 🗺️\n");
    printf("Find clues, collect treasures, and discover the final prize!\n\n");
    
    // Setup the treasure hunt
    setupHunt(game);
    
    do {
        printf("\n=== TREASURE HUNT MENU ===\n");
        printf("1. Add player\n");
        printf("2. Explore current location\n");
        printf("3. Move to next location\n");
        printf("4. Show inventory\n");
        printf("5. Show all players\n");
        printf("6. Show game map\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter player name: ");
                fgets(playerName, sizeof(playerName), stdin);
                playerName[strcspn(playerName, "\n")] = 0;
                
                printf("Enter player ID: ");
                scanf("%d", &playerId);
                
                addPlayer(game, playerName, playerId);
                break;
                
            case 2:
                printf("Enter player ID: ");
                scanf("%d", &playerId);
                
                currentPlayer = findPlayer(game, playerId);
                if (currentPlayer != NULL) {
                    exploreLocation(currentPlayer);
                } else {
                    printf("Player not found!\n");
                }
                break;
                
            case 3:
                printf("Enter player ID: ");
                scanf("%d", &playerId);
                
                currentPlayer = findPlayer(game, playerId);
                if (currentPlayer != NULL) {
                    moveToNextLocation(currentPlayer);
                } else {
                    printf("Player not found!\n");
                }
                break;
                
            case 4:
                printf("Enter player ID: ");
                scanf("%d", &playerId);
                
                currentPlayer = findPlayer(game, playerId);
                if (currentPlayer != NULL) {
                    showInventory(currentPlayer);
                } else {
                    printf("Player not found!\n");
                }
                break;
                
            case 5:
                showAllPlayers(game);
                break;
                
            case 6:
                showGameMap(game);
                break;
                
            case 7:
                printf("Thanks for playing the Treasure Hunt! 🏴‍☠️\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);
    
    freeGame(game);
    return 0;
}