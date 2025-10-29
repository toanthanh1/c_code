/*
 * Maze Solver with Backtracking using Stack
 * This program solves a maze using stack-based backtracking algorithm.
 * The stack keeps track of the path and backtracks when dead ends are reached.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 20
#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'
#define VISITED '.'
#define SOLUTION '*'

// Structure to represent a position in the maze
typedef struct Position {
    int row;
    int col;
    struct Position* next;
} Position;

// Structure to represent the stack for backtracking
typedef struct Stack {
    Position* top;
    int size;
} Stack;

// Structure to represent the maze
typedef struct Maze {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
    Position start;
    Position end;
    int solutionFound;
    int steps;
} Maze;

// Function to create a new position
Position* createPosition(int row, int col) {
    Position* pos = (Position*)malloc(sizeof(Position));
    pos->row = row;
    pos->col = col;
    pos->next = NULL;
    return pos;
}

// Function to create a stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

// Function to check if stack is empty
int isStackEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Function to push position onto stack
void push(Stack* stack, int row, int col) {
    Position* newPos = createPosition(row, col);
    newPos->next = stack->top;
    stack->top = newPos;
    stack->size++;
}

// Function to pop position from stack
Position* pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        return NULL;
    }
    
    Position* pos = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    return pos;
}

// Function to peek at top position without removing
Position* peek(Stack* stack) {
    return stack->top;
}

// Function to create a maze
Maze* createMaze(int rows, int cols) {
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    maze->rows = rows;
    maze->cols = cols;
    maze->solutionFound = 0;
    maze->steps = 0;
    
    // Initialize with walls
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze->grid[i][j] = WALL;
        }
    }
    
    return maze;
}

// Function to display the maze
void displayMaze(Maze* maze) {
    printf("\n");
    // Print column numbers
    printf("   ");
    for (int j = 0; j < maze->cols; j++) {
        printf("%2d", j);
    }
    printf("\n");
    
    for (int i = 0; i < maze->rows; i++) {
        printf("%2d ", i); // Print row number
        for (int j = 0; j < maze->cols; j++) {
            printf(" %c", maze->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to check if a position is valid
int isValidPosition(Maze* maze, int row, int col) {
    return (row >= 0 && row < maze->rows && 
            col >= 0 && col < maze->cols);
}

// Function to check if a position can be moved to
int canMoveTo(Maze* maze, int row, int col) {
    return (isValidPosition(maze, row, col) && 
            (maze->grid[row][col] == PATH || 
             maze->grid[row][col] == END));
}

// Function to load a predefined maze
void loadSampleMaze(Maze* maze) {
    char sampleMaze[10][21] = {
        "####################",
        "#S       #         #",
        "# ##### ### ####### #",
        "# #   # #   #     # #",
        "# # # # # ### ### # #",
        "#   #   #   #   #   #",
        "####### # # # #######",
        "#     #   # #   #   #",
        "# ### ##### ### # # #",
        "#   #           #  E#"
    };
    
    maze->rows = 10;
    maze->cols = 20;
    
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            maze->grid[i][j] = sampleMaze[i][j];
            if (sampleMaze[i][j] == START) {
                maze->start.row = i;
                maze->start.col = j;
            } else if (sampleMaze[i][j] == END) {
                maze->end.row = i;
                maze->end.col = j;
            }
        }
    }
    
    printf("Sample maze loaded!\n");
    printf("Start: (%d, %d), End: (%d, %d)\n", 
           maze->start.row, maze->start.col,
           maze->end.row, maze->end.col);
}

// Function to generate a random maze
void generateRandomMaze(Maze* maze, int rows, int cols) {
    maze->rows = rows;
    maze->cols = cols;
    
    srand(time(NULL));
    
    // Fill with random walls and paths
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows-1 || j == 0 || j == cols-1) {
                maze->grid[i][j] = WALL; // Border walls
            } else {
                maze->grid[i][j] = (rand() % 3 == 0) ? WALL : PATH;
            }
        }
    }
    
    // Set start and end positions
    maze->start.row = 1;
    maze->start.col = 1;
    maze->grid[1][1] = START;
    
    maze->end.row = rows - 2;
    maze->end.col = cols - 2;
    maze->grid[rows-2][cols-2] = END;
    
    // Ensure there's a path from start
    maze->grid[1][2] = PATH;
    maze->grid[2][1] = PATH;
    
    // Ensure there's a path to end
    maze->grid[rows-2][cols-3] = PATH;
    maze->grid[rows-3][cols-2] = PATH;
    
    printf("Random maze generated!\n");
}

// Function to solve maze using backtracking with stack
int solveMaze(Maze* maze) {
    Stack* pathStack = createStack();
    
    // Directions: up, right, down, left
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    char dirNames[4][10] = {"UP", "RIGHT", "DOWN", "LEFT"};
    
    // Start from the starting position
    push(pathStack, maze->start.row, maze->start.col);
    maze->grid[maze->start.row][maze->start.col] = VISITED;
    
    printf("\n=== MAZE SOLVING PROCESS ===\n");
    printf("Starting from position (%d, %d)\n", maze->start.row, maze->start.col);
    
    while (!isStackEmpty(pathStack)) {
        Position* current = peek(pathStack);
        maze->steps++;
        
        printf("\nStep %d: Current position (%d, %d)\n", maze->steps, current->row, current->col);
        
        // Check if we reached the end
        if (current->row == maze->end.row && current->col == maze->end.col) {
            printf("MAZE SOLVED!\n");
            maze->solutionFound = 1;
            
            // Mark the solution path
            Position* pathPos = pathStack->top;
            while (pathPos != NULL) {
                if (maze->grid[pathPos->row][pathPos->col] != START && 
                    maze->grid[pathPos->row][pathPos->col] != END) {
                    maze->grid[pathPos->row][pathPos->col] = SOLUTION;
                }
                pathPos = pathPos->next;
            }
            
            return 1;
        }
        
        // Try all four directions
        int moved = 0;
        for (int i = 0; i < 4; i++) {
            int newRow = current->row + directions[i][0];
            int newCol = current->col + directions[i][1];
            
                printf("  Trying %s: (%d, %d) ", dirNames[i], newRow, newCol);
            
            if (canMoveTo(maze, newRow, newCol)) {
                printf("Valid move\n");
                push(pathStack, newRow, newCol);
                if (maze->grid[newRow][newCol] != END) {
                    maze->grid[newRow][newCol] = VISITED;
                }
                moved = 1;
                break;
            } else {
                if (!isValidPosition(maze, newRow, newCol)) {
                    printf("Out of bounds\n");
                } else if (maze->grid[newRow][newCol] == WALL) {
                    printf("Wall\n");
                } else if (maze->grid[newRow][newCol] == VISITED) {
                    printf("Already visited\n");
                } else {
                    printf("Invalid\n");
                }
            }
        }
        
        if (!moved) {
            // Dead end - backtrack
            Position* deadEnd = pop(pathStack);
            printf("  Dead end! Backtracking from (%d, %d)\n", deadEnd->row, deadEnd->col);
            
            if (maze->grid[deadEnd->row][deadEnd->col] != START) {
                maze->grid[deadEnd->row][deadEnd->col] = PATH; // Unmark visited
            }
            
            free(deadEnd);
            
            if (!isStackEmpty(pathStack)) {
                Position* prev = peek(pathStack);
                printf("  Returned to (%d, %d)\n", prev->row, prev->col);
            }
        }
        
        // Optional: Show maze state every few steps
        if (maze->steps % 5 == 0 || moved) {
            printf("  Current maze state:\n");
            displayMaze(maze);
        }
    }
    
    printf("No solution found!\n");
    return 0;
}

// Function to display solution path
void displaySolutionPath(Maze* maze) {
    if (!maze->solutionFound) {
        printf("No solution to display!\n");
        return;
    }
    
    printf("\n=== SOLUTION MAZE ===\n");
    printf("Legend: S=Start, E=End, *=Solution Path, #=Wall, ' '=Open Path\n");
    displayMaze(maze);
    
    // Count solution path length
    int pathLength = 0;
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            if (maze->grid[i][j] == SOLUTION) {
                pathLength++;
            }
        }
    }
    
    printf("Solution path length: %d steps\n", pathLength);
    printf("Total exploration steps: %d\n", maze->steps);
}

// Function to reset maze for new solving attempt
void resetMaze(Maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            if (maze->grid[i][j] == VISITED || maze->grid[i][j] == SOLUTION) {
                maze->grid[i][j] = PATH;
            }
        }
    }
    
    maze->grid[maze->start.row][maze->start.col] = START;
    maze->grid[maze->end.row][maze->end.col] = END;
    maze->solutionFound = 0;
    maze->steps = 0;
    
    printf("Maze reset for new solving attempt!\n");
}

// Function to edit maze manually
void editMaze(Maze* maze) {
    int row, col;
    char cell;
    
    printf("Current maze:\n");
    displayMaze(maze);
    
    printf("Enter position to edit (row col): ");
    scanf("%d %d", &row, &col);
    
    if (!isValidPosition(maze, row, col)) {
        printf("Invalid position!\n");
        return;
    }
    
    printf("Enter new cell value (#=wall, ' '=path): ");
    scanf(" %c", &cell);
    
    if (cell == WALL || cell == PATH) {
        maze->grid[row][col] = cell;
        printf("Cell (%d, %d) updated!\n", row, col);
    } else {
        printf("Invalid cell value!\n");
    }
}

// Function to free stack memory
void freeStack(Stack* stack) {
    while (!isStackEmpty(stack)) {
        Position* pos = pop(stack);
        free(pos);
    }
    free(stack);
}

// Main function with menu-driven interface
int main() {
    Maze* maze = createMaze(10, 20);
    int choice, rows, cols;
    
    printf("Welcome to Maze Solver with Backtracking!\n");
    printf("Navigate through mazes using stack-based pathfinding!\n\n");
    
    do {
        printf("\n=== MAZE SOLVER MENU ===\n");
        printf("1. Load sample maze\n");
        printf("2. Generate random maze\n");
        printf("3. Display current maze\n");
        printf("4. Solve maze\n");
        printf("5. Display solution\n");
        printf("6. Reset maze\n");
        printf("7. Edit maze manually\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                loadSampleMaze(maze);
                displayMaze(maze);
                break;
                
            case 2:
                printf("Enter maze dimensions (rows cols, max %d): ", MAX_SIZE);
                scanf("%d %d", &rows, &cols);
                
                if (rows > 3 && rows <= MAX_SIZE && cols > 3 && cols <= MAX_SIZE) {
                    generateRandomMaze(maze, rows, cols);
                    displayMaze(maze);
                } else {
                    printf("Invalid dimensions! Must be 4-%d for both.\n", MAX_SIZE);
                }
                break;
                
            case 3:
                displayMaze(maze);
                break;
                
            case 4:
                if (maze->rows == 0) {
                    printf("No maze loaded! Please load or generate a maze first.\n");
                } else {
                    resetMaze(maze); // Reset before solving
                    solveMaze(maze);
                }
                break;
                
            case 5:
                displaySolutionPath(maze);
                break;
                
            case 6:
                resetMaze(maze);
                break;
                
            case 7:
                editMaze(maze);
                break;
                
            case 8:
                printf("Thanks for using Maze Solver!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
    
    free(maze);
    return 0;
}