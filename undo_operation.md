# undo_operation.c

Description

Demonstrates undo/redo functionality using stacks: push operations on do-stack, pop to undo, push undone on redo-stack.

Features

- Perform operations (example: text edits)
- Undo and redo
- Show current state

Compile (Windows PowerShell)

```powershell
gcc -o undo_operation.exe undo_operation.c
.\undo_operation.exe
```

Usage

Run the program and use menu to perform actions, undo, and redo.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Undo/Redo System"
        Init["Initialize:
        - Do Stack
        - Redo Stack
        - Current State"]
        
        subgraph "Action Structure"
            direction LR
            Action["Action Data:
            - Operation type
            - Parameters
            - Reverse operation"]
            
            State["State Tracking:
            - Current state
            - Action history
            - Change metadata"]
        end
        
        subgraph "Core Operations"
            Do["Perform Action:
            1. Execute operation
            2. Push to do stack
            3. Clear redo stack"]
            
            Undo["Undo Action:
            1. Pop from do stack
            2. Reverse operation
            3. Push to redo stack"]
            
            Redo["Redo Action:
            1. Pop from redo stack
            2. Re-apply operation
            3. Push to do stack"]
        end
    end

    Init --> Action
    Action --> State
    
    State --> Operation{"User
    Operation"}
    
    Operation -->|"New Action"| Do
    Operation -->|"Undo"| Check1{"Can Undo?"}
    Operation -->|"Redo"| Check2{"Can Redo?"}
    
    Check1 -->|Yes| Undo
    Check1 -->|No| Error1["Nothing
    to undo"]
    
    Check2 -->|Yes| Redo
    Check2 -->|No| Error2["Nothing
    to redo"]
    
    Do --> Update["Update State:
    - Apply changes
    - Update history"]
    Undo --> Update
    Redo --> Update
    
    Update --> End([End])
    Error1 --> End
    Error2 --> End
```

Algorithm explanation:
1. Stack Management:
   - Two stacks for bidirectional history
   - Each action stores reverse operation
   - Clear redo on new action
2. Core Operations:
   - Do: Execute and store
   - Undo: Reverse last action
   - Redo: Reapply undone action
3. State Handling:
   - Track current state
   - Maintain action history
   - Validate operations

Notes

- Similar to text editor undo/redo
- Can be extended for complex operations