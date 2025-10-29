# balanced_parentheses.c

Description

Checks if an input string has balanced parentheses/brackets/braces using a stack. Typical problem for validating expressions.

Features

- Supports (), [], {}
- Uses stack to push opening brackets and pop on matching closing brackets

Compile (Windows PowerShell)

```powershell
gcc -o balanced_parentheses.exe balanced_parentheses.c
.\balanced_parentheses.exe
```

Usage

Provide an expression when prompted; program outputs whether parentheses are balanced.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Matching Algorithm"
        Init["Initialize Empty Stack"]
        
        subgraph "Character Processing"
            direction LR
            Char["Get Next Character"]
            IsOpen{"Opening Bracket?
            (, [, {"}
            IsClose{"Closing Bracket?
            ), ], }"}
        end
        
        subgraph "Stack Operations"
            Push["Push to Stack:
            Store opening bracket"]
            
            Match{"Stack Top Matches?
            Compare pairs:
            ( with )
            [ with ]
            { with }"}
            
            Pop["Pop from Stack:
            Remove matched pair"]
        end
    end

    Init --> Char
    Char --> IsOpen
    IsOpen -->|Yes| Push
    Push --> Char
    
    IsOpen -->|No| IsClose
    IsClose -->|Yes| Match
    IsClose -->|No| Char
    
    Match -->|Yes| Pop
    Match -->|No| Invalid["Not Balanced:
    Mismatched brackets"]
    
    Pop --> Char
    
    Char -->|End of String| Final{"Stack Empty?"}
    Final -->|Yes| Valid["Balanced:
    All brackets matched"]
    Final -->|No| Invalid
    
    Valid --> End([End])
    Invalid --> End
```

Algorithm explanation:
1. Use stack for bracket matching:
   - Push opening brackets onto stack
   - Match closing brackets with stack top
   - Pop when pairs match correctly
2. Key validations:
   - Stack empty for closing bracket = Invalid
   - Mismatched brackets = Invalid
   - Non-empty stack at end = Invalid
3. Time complexity: O(n)
   - Single pass through string
   - Constant time operations

Notes

- Handle edge cases: empty string, non-bracket chars ignored
- Stack ensures proper nesting order