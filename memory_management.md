# memory_management.c

Description

Demonstrates manual memory management techniques in C: allocation, deallocation, tracking allocations, and detecting leaks.

Features

- Allocate/free blocks
- Track active allocations
- Optionally simulate simple allocator

Compile (Windows PowerShell)

```powershell
gcc -o memory_management.exe memory_management.c
.\memory_management.exe
```

Usage

Run and follow menu to allocate and free memory. Use provided tracking output to inspect leaks.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Memory Management"
        Init["Initialize Memory Tracker:
        - Block list
        - Statistics counters"]
        
        subgraph "Memory Block Structure"
            direction LR
            Block["Memory Block:
            - Address
            - Size
            - Allocation time
            - Source location"]
        end
        
        subgraph "Core Operations"
            Alloc["Memory Allocation:
            1. Request memory
            2. Track block info
            3. Update statistics"]
            
            Free["Memory Deallocation:
            1. Validate pointer
            2. Release memory
            3. Remove tracking"]
            
            Track["Memory Tracking:
            1. List active blocks
            2. Calculate total usage
            3. Detect potential leaks"]
        end
    end

    Init --> Block
    Block --> Alloc
    
    Alloc --> Check{"Allocation
    Successful?"}
    Check -->|Yes| Record["Record Block:
    Add to tracking list"]
    Check -->|No| Error["Handle
    Out of Memory"]
    
    Record --> Track
    Free --> Track
    
    Track --> Leak{"Memory
    Leak Check"}
    Leak -->|Found| Report["Report:
    Unfreed blocks"]
    Leak -->|None| Clean["Memory
    Status: Clean"]
    
    Report --> End([End])
    Clean --> End
    Error --> End
```

Algorithm explanation:
1. Memory Block Tracking:
   - Track each allocation's metadata
   - Maintain linked list of active blocks
   - Record allocation details for debugging
2. Core Operations:
   - Allocation: malloc + tracking
   - Deallocation: free + cleanup
   - Leak detection: analyze active blocks
3. Memory Safety:
   - Validate all pointers
   - Track allocation sizes
   - Monitor total memory usage

Notes

- This program is educational; use Valgrind (on Linux) for real leak detection.
- Focus on understanding memory lifecycle and common issues.