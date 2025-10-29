/*
 * Elimination Game (Josephus Problem) using Circular Linked List
 * This program implements a game where players are eliminated in a circle
 * by counting every kth player until only one remains (Josephus Problem).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a player in the game
typedef struct Player {
    int playerNumber;
    char name[50];
    int isActive;
    struct Player* next;
} Player;

// Structure to represent the elimination game
typedef struct EliminationGame {
    Player* head;
    Player* currentPlayer;
    int totalPlayers;
    int activePlayers;
    int eliminationCount; // k value - every kth player is eliminated
    int round;
} EliminationGame;

// Function to create a new player
Player* createPlayer(int number, char* name) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    newPlayer->playerNumber = number;
    strcpy(newPlayer->name, name);
    newPlayer->isActive = 1;
    newPlayer->next = NULL;
    return newPlayer;
}

// Function to create the elimination game
EliminationGame* createGame(int k) {
    EliminationGame* game = (EliminationGame*)malloc(sizeof(EliminationGame));
    game->head = NULL;
    game->currentPlayer = NULL;
    game->totalPlayers = 0;
    game->activePlayers = 0;
    game->eliminationCount = k;
    game->round = 1;
    return game;
}

// Function to add a player to the circular linked list
void addPlayer(EliminationGame* game, int number, char* name) {
    Player* newPlayer = createPlayer(number, name);
    
    if (game->head == NULL) {
        // First player - create circular list
        game->head = newPlayer;
        newPlayer->next = newPlayer; // Points to itself
        game->currentPlayer = newPlayer;
    } else {
        // Find the last player and insert new player
        Player* current = game->head;
        while (current->next != game->head) {
            current = current->next;
        }
        
        // Insert new player between last and first
        current->next = newPlayer;
        newPlayer->next = game->head;
    }
    
    game->totalPlayers++;
    game->activePlayers++;
    printf("Player %d (%s) joined the game!\n", number, name);
}

// Function to setup players automatically
void setupPlayers(EliminationGame* game, int numPlayers) {
    char name[50];
    for (int i = 1; i <= numPlayers; i++) {
        sprintf(name, "Player%d", i);
        addPlayer(game, i, name);
    }
    printf("\n%d players have been added to the game!\n", numPlayers);
}

// Function to display all players in the circle
void displayCircle(EliminationGame* game) {
    if (game->head == NULL) {
        printf("No players in the game!\n");
        return;
    }
    
    printf("\n=== PLAYER CIRCLE (Round %d) ===\n", game->round);
    printf("Active players: %d\n", game->activePlayers);
    printf("Elimination count: Every %d player(s)\n", game->eliminationCount);
    printf("Current position: %s\n\n", game->currentPlayer->name);
    
    Player* current = game->head;
    printf("Circle: ");
    
    do {
        if (current->isActive) {
            printf("%s(%d)", current->name, current->playerNumber);
            if (current == game->currentPlayer) {
                printf("[*]");
            }
            printf(" -> ");
        }
        current = current->next;
    } while (current != game->head);
    
    printf("(back to start)\n\n");
}

// Function to move to next active player
Player* getNextActivePlayer(Player* current) {
    Player* next = current->next;
    while (!next->isActive) {
        next = next->next;
    }
    return next;
}

// Function to eliminate a player
void eliminatePlayer(EliminationGame* game, Player* playerToEliminate) {
    playerToEliminate->isActive = 0;
    game->activePlayers--;
    
    printf("🔥 Player %d (%s) has been eliminated! 🔥\n", 
           playerToEliminate->playerNumber, playerToEliminate->name);
    printf("Remaining players: %d\n\n", game->activePlayers);
    
    // Move current pointer to next active player
    if (game->activePlayers > 0) {
        game->currentPlayer = getNextActivePlayer(playerToEliminate);
    }
}

// Function to count k steps and eliminate player
void performElimination(EliminationGame* game) {
    if (game->activePlayers <= 1) {
        printf("Game over! Cannot eliminate more players.\n");
        return;
    }
    
    printf("\n--- Round %d ---\n", game->round);
    printf("Starting from: %s\n", game->currentPlayer->name);
    printf("Counting %d steps...\n", game->eliminationCount);
    
    // Count k-1 steps (since we start counting from current position)
    Player* current = game->currentPlayer;
    
    for (int count = 1; count < game->eliminationCount; count++) {
        current = getNextActivePlayer(current);
        printf("Step %d: %s\n", count + 1, current->name);
    }
    
    printf("Player to eliminate: %s\n", current->name);
    eliminatePlayer(game, current);
    game->round++;
}

// Function to play the entire game automatically
void playEntireGame(EliminationGame* game) {
    printf("\n🎮 Starting the Elimination Game! 🎮\n");
    printf("Rules: Every %d player will be eliminated\n\n", game->eliminationCount);
    
    while (game->activePlayers > 1) {
        displayCircle(game);
        performElimination(game);
        
        // Pause for dramatic effect (remove in real implementation)
        printf("Press Enter to continue...");
        getchar();
    }
    
    // Find the winner
    Player* current = game->head;
    while (!current->isActive) {
        current = current->next;
    }
    
    printf("\n🏆 WINNER! 🏆\n");
    printf("Player %d (%s) is the last remaining player!\n", 
           current->playerNumber, current->name);
    printf("Total rounds: %d\n", game->round - 1);
}

// Function to manually step through the game
void stepThroughGame(EliminationGame* game) {
    if (game->activePlayers <= 1) {
        Player* current = game->head;
        while (!current->isActive) {
            current = current->next;
        }
        printf("Game already finished! Winner: %s\n", current->name);
        return;
    }
    
    displayCircle(game);
    performElimination(game);
    
    if (game->activePlayers == 1) {
        Player* current = game->head;
        while (!current->isActive) {
            current = current->next;
        }
        printf("\n🏆 GAME FINISHED! 🏆\n");
        printf("Winner: Player %d (%s)\n", current->playerNumber, current->name);
    }
}

// Function to reset the game
void resetGame(EliminationGame* game) {
    Player* current = game->head;
    if (current != NULL) {
        do {
            current->isActive = 1;
            current = current->next;
        } while (current != game->head);
    }
    
    game->activePlayers = game->totalPlayers;
    game->currentPlayer = game->head;
    game->round = 1;
    printf("Game reset! All players are active again.\n");
}

// Function to change elimination count
void changeEliminationCount(EliminationGame* game, int newK) {
    game->eliminationCount = newK;
    printf("Elimination count changed to: %d\n", newK);
}

// Function to show game statistics
void showStatistics(EliminationGame* game) {
    printf("\n=== GAME STATISTICS ===\n");
    printf("Total players: %d\n", game->totalPlayers);
    printf("Active players: %d\n", game->activePlayers);
    printf("Eliminated players: %d\n", game->totalPlayers - game->activePlayers);
    printf("Current round: %d\n", game->round);
    printf("Elimination count (k): %d\n", game->eliminationCount);
    
    if (game->currentPlayer != NULL) {
        printf("Current position: %s\n", game->currentPlayer->name);
    }
    
    printf("Game status: ");
    if (game->activePlayers > 1) {
        printf("In progress\n");
    } else if (game->activePlayers == 1) {
        printf("Finished\n");
    } else {
        printf("Not started\n");
    }
    printf("\n");
}

// Function to free the game memory
void freeGame(EliminationGame* game) {
    if (game->head != NULL) {
        Player* current = game->head;
        Player* next;
        
        do {
            next = current->next;
            free(current);
            current = next;
        } while (current != game->head);
    }
    
    free(game);
}

// Main function with menu-driven interface
int main() {
    EliminationGame* game = NULL;
    int choice, numPlayers, k, newK;
    
    printf("🎯 Welcome to the Elimination Game (Josephus Problem)! 🎯\n");
    printf("Players stand in a circle and every kth player is eliminated.\n");
    printf("The last remaining player wins!\n\n");
    
    do {
        printf("\n=== ELIMINATION GAME MENU ===\n");
        printf("1. Create new game\n");
        printf("2. Display player circle\n");
        printf("3. Step through game (one elimination)\n");
        printf("4. Play entire game automatically\n");
        printf("5. Reset game\n");
        printf("6. Change elimination count\n");
        printf("7. Show statistics\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                if (game != NULL) {
                    freeGame(game);
                }
                
                printf("Enter number of players: ");
                scanf("%d", &numPlayers);
                
                printf("Enter elimination count (k): ");
                scanf("%d", &k);
                
                if (numPlayers < 2 || k < 1) {
                    printf("Invalid input! Need at least 2 players and k >= 1\n");
                    break;
                }
                
                game = createGame(k);
                setupPlayers(game, numPlayers);
                break;
                
            case 2:
                if (game != NULL) {
                    displayCircle(game);
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 3:
                if (game != NULL) {
                    stepThroughGame(game);
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 4:
                if (game != NULL) {
                    playEntireGame(game);
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 5:
                if (game != NULL) {
                    resetGame(game);
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 6:
                if (game != NULL) {
                    printf("Enter new elimination count: ");
                    scanf("%d", &newK);
                    if (newK >= 1) {
                        changeEliminationCount(game, newK);
                    } else {
                        printf("Invalid elimination count! Must be >= 1\n");
                    }
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 7:
                if (game != NULL) {
                    showStatistics(game);
                } else {
                    printf("No game created yet!\n");
                }
                break;
                
            case 8:
                printf("Thanks for playing the Elimination Game!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
    
    if (game != NULL) {
        freeGame(game);
    }
    
    return 0;
}