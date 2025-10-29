# string_reversal.c

Description

Reverses input strings using stack or in-place swapping. Educational example for string manipulation.

Features

- Reverse string using stack or two-pointer method
- Handle newline/whitespace

Compile (Windows PowerShell)

```powershell
gcc -o string_reversal.exe string_reversal.c
.\string_reversal.exe
```

Usage

Enter a string when prompted; program prints the reversed string.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Input["Input String"]

    subgraph "Core Reversal Strategies"
        subgraph "Stack Method"
            direction LR
            Stack["Initialize
            Empty Stack"]
            
            PushPhase["Push Phase:
            Push each char
            onto stack"]
            
            PopPhase["Pop Phase:
            Pop to build
            reversed string"]
        end
        
        subgraph "Two-Pointer Method"
            direction LR
            Init["Initialize:
            left = start
            right = end"]
            
            Swap["Swap Characters:
            temp = str[left]
            str[left] = str[right]
            str[right] = temp"]
            
            Move["Move Pointers:
            left++
            right--"]
        end
    end

    Input --> Method{"Choose Method"}
    
    Method -->|Stack Based| Stack
    Stack --> PushPhase
    PushPhase --> PopPhase
    PopPhase --> Result1["Reversed
    String"]
    
    Method -->|In-Place| Init
    Init --> Check{"left < right?"}
    Check -->|Yes| Swap
    Swap --> Move
    Move --> Check
    Check -->|No| Result2["Reversed
    String"]
    
    Result1 --> End([End])
    Result2 --> End
```

Algorithm explanation:
1. Stack-based reversal:
   - Space: O(n)
   - Time: O(n)
   - Non-destructive to original
2. Two-pointer reversal:
   - Space: O(1)
   - Time: O(n)
   - Modifies original string
3. Key differences:
   - Stack preserves original
   - Two-pointer is memory efficient

Notes

- Handle empty string and unicode characters carefully
- Consider string length for method selection