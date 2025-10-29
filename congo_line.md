# congo_line.c

Description

Simulates a conga line or queue behavior, often used to demonstrate queue operations (enqueue, dequeue) in C.

Features

- Enqueue and dequeue operations
- Display queue
- Circular queue or linked list-based queue depending on implementation

Compile (Windows PowerShell)

```powershell
gcc -o congo_line.exe congo_line.c
.\congo_line.exe
```

Usage

Run and follow menu to add/remove participants and view the line.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Queue Structure"
        Init["Initialize Queue:
        front = NULL
        rear = NULL"]
        
        subgraph "Node Structure"
            direction LR
            Node["Dancer Node:
            - ID/Name
            - Next pointer"]
        end
        
        subgraph "Core Operations"
            Enqueue["Join Line (Enqueue):
            1. Create new node
            2. Link to rear
            3. Update rear pointer"]
            
            Dequeue["Leave Line (Dequeue):
            1. Save front node
            2. Update front pointer
            3. Free leaving node"]
            
            Special["Special Moves:
            - Move to front
            - Swap positions
            - Group formations"]
        end
    end

    Init --> Node
    Node --> State{"Queue State"}
    
    State -->|"Empty"| FirstJoin["Handle First Dancer:
    front = rear = new node"]
    
    State -->|"Has Dancers"| Operation{"Choose Operation"}
    
    Operation -->|"Join"| Enqueue
    Operation -->|"Leave"| Dequeue
    Operation -->|"Special"| Special
    
    FirstJoin --> Maintain["Maintain Queue:
    - Update pointers
    - Handle edge cases
    - Verify structure"]
    
    Enqueue --> Maintain
    Dequeue --> Maintain
    Special --> Maintain
    
    Maintain --> End([End])
```

Algorithm explanation:
1. Queue Implementation:
   - Linked list for dynamic size
   - Front/rear pointers for O(1) access
   - Special move operations
2. Core Operations:
   - Enqueue: O(1) at rear
   - Dequeue: O(1) at front
   - Special moves: O(n) search + O(1) update
3. Edge Cases:
   - Empty queue handling
   - Single dancer operations
   - Full queue scenarios

Notes

- Linked list allows dynamic line size
- Special moves maintain queue integrity