# directory_history.c

Description

Tracks navigation history of directories (like a shell history), demonstrating stack/linked-list usage to go back/forward in history.

Features

- Push directory on navigation
- Back/forward operations
- Display history

Compile (Windows PowerShell)

```powershell
gcc -o directory_history.exe directory_history.c
.\directory_history.exe
```

Usage

Run and use menu: navigate to new directory (push), go back (pop), view history.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core History Structure"
        Init["Initialize:
        - Back stack
        - Forward stack
        - Current directory"]
        
        subgraph "Stack Components"
            direction LR
            Node["Directory Node:
            - Path
            - Timestamp
            - Next pointer"]
            
            Stacks["Dual Stack System:
            Back: Previous dirs
            Forward: Undone moves"]
        end
        
        subgraph "Core Operations"
            Navigate["Navigate:
            1. Validate new path
            2. Push current to back
            3. Clear forward stack"]
            
            Back["Go Back:
            1. Push current to forward
            2. Pop from back stack
            3. Update current"]
            
            Forward["Go Forward:
            1. Push current to back
            2. Pop from forward
            3. Update current"]
        end
    end

    Init --> Node
    Node --> Stacks
    
    Stacks --> Action{"User Action"}
    
    Action -->|"New Path"| Navigate
    Action -->|"Back"| Back
    Action -->|"Forward"| Forward
    
    Navigate --> Update["Update State:
    - Current directory
    - Stack contents"]
    Back --> Update
    Forward --> Update
    
    Update --> End([End])
```

Algorithm explanation:
1. Data Structure:
   - Two stacks for bidirectional history
   - O(1) operations for all actions
   - Efficient memory management
2. Core Operations:
   - Navigate: Clear forward history
   - Back/Forward: Move between stacks
   - All operations maintain consistency
3. Path Handling:
   - Validate directories
   - Normalize paths
   - Error checking

Notes

- Similar to browser history mechanics
- Can be extended to persist between sessions