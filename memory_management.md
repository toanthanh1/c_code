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

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Init allocation tracker]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Allocate| Alloc[malloc -> record allocation]
  Menu -->|Free| Free[free -> remove record]
  Menu -->|Report| Report[Iterate tracker -> show leaks]
  Alloc --> Menu
  Free --> Menu
  Report --> Menu
  Menu --> Exit[Exit -> free remaining allocations]
  Exit --> End([End])
```

Notes

- This program is educational; use Valgrind (on Linux) for real leak detection.

## Memory Management Algorithm

```mermaid
flowchart TD
    A([Start]) --> B[Initialize tracker]
    B --> C{Operation?}
    C -->|Allocate| D[Call malloc]
    D --> E[Add to list]
    C -->|Free| F[Find block]
    F --> G[Call free]
    G --> H[Remove from list]
    C -->|Check| I[/Print unfreed blocks/]
    H --> C
    E --> C
    I --> C
    C -->|Exit| J([End])
    
    TryAlloc --> Success{Successful?}
    Success -->|Yes| Record["block = new Block
    block.addr = ptr
    block.size = size
    block.time = now()"]
    
    Record --> UpdateStats["totalAlloc += size
    blockCount++"]
    
    Success -->|No| Error1[/Error: out of memory/]
    
    Menu -->|2. Free| GetPtr[/Input pointer/]
    GetPtr --> FindBlock["block = find in list"]
    
    FindBlock --> Found{Found?}
    Found -->|Yes| Free["free(block.addr)
    totalAlloc -= block.size
    blockCount--"]
    
    Found -->|No| Error2[/Error: invalid pointer/]
    
    Menu -->|3. Check| CheckLeak["For each block:
    - Print address
    - Print size
    - Print time"]
    
    CheckLeak --> Stats[/"Totals:
    - Block count: blockCount
    - Memory: totalAlloc"/]
    
    Menu -->|4| Exit([End])
```

Algorithm explanation:
1. Allocation:
   - malloc() + check success
   - Create new block record
   - Update statistics
2. Deallocation:
   - Find block in list
   - free() if found
   - Update statistics
3. Leak checking:
   - List all unfreed blocks
   - Show total statistics
        CreateTracker --> InitList[Initialize blocks list]
    end

    subgraph "Allocation Management"
        Alloc([Allocate]) --> CallMalloc[Call malloc()]
        CallMalloc --> CreateRecord[Create new record]
        CreateRecord --> FillInfo[Store size, address, time]
        FillInfo --> AddToList[Add to blocks list]
    end

    subgraph "Deallocation"
        Free([Free]) --> FindBlock[Find block in list]
        FindBlock --> ValidPtr{Valid pointer?}
        ValidPtr -->|Yes| CallFree[Call free()]
        ValidPtr -->|No| Error[Report error]
        CallFree --> RemoveRecord[Remove from list]
    end

    subgraph "Leak Detection"
        Check([Check]) --> ScanList[Scan blocks list]
        ScanList --> FilterOld[Filter old blocks]
        FilterOld --> ReportLeaks[Report leaks]
    end
```
