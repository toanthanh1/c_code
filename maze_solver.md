# maze_solver.c

Description

Solves mazes using search algorithms (DFS, BFS, A* optionally). Demonstrates grid traversal, visited marking, and path reconstruction.

Features

- Read maze from input or file
- Solve using BFS/DFS
- Print solved path and steps

Compile (Windows PowerShell)

```powershell
gcc -o maze_solver.exe maze_solver.c
.\maze_solver.exe
```

Usage

Provide maze input (or file). Program outputs path if exists.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Maze Solving"
        Init["Initialize:
        - Visited matrix
        - Parent matrix
        - Search queue/stack"]
        
        subgraph "Search Strategy"
            direction LR
            BFS["Breadth-First Search:
            Queue for shortest path"]
            DFS["Depth-First Search:
            Stack for memory efficiency"]
        end
        
        subgraph "Cell Processing"
            Current["Get current cell
            from queue/stack"]
            
            Valid{"Valid move?
            1. Within bounds
            2. Not a wall
            3. Not visited"}
            
            Process["For valid cell:
            1. Mark as visited
            2. Store parent
            3. Add to queue/stack"]
        end
        
        Check{"Goal found?"}
    end

    Init --> Current
    Current --> Valid
    Valid -->|Yes| Process
    Valid -->|No| Current
    Process --> Check
    Check -->|No| Current
    Check -->|Yes| Path["Reconstruct Path:
    Follow parent pointers
    from goal to start"]
    
    Path --> End([End])
```

Algorithm explanation:
1. Core components:
   - Visited matrix prevents cycles
   - Parent matrix tracks path
   - Queue/Stack for search frontier
2. Key operations:
   - Check cell validity
   - Mark and track visited cells
   - Store path information
3. Path reconstruction:
   - Follow parent pointers
   - Build path in reverse

Notes

- BFS guarantees shortest path
- DFS uses less memory
- For larger mazes, consider A* with heuristics