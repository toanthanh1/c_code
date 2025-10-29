# reservation_counter.c

Description

Implements a reservation/counter system—could be used to demonstrate counting resources, booking slots, or semaphore-like behavior.

Features

- Reserve (decrement available)
- Release (increment available)
- Display current availability

Compile (Windows PowerShell)

```powershell
gcc -o reservation_counter.exe reservation_counter.c
.\reservation_counter.exe
```

Usage

Run and interact via menu to reserve or release resources.

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Counter System"
        Init["Initialize Counter:
        - Maximum capacity
        - Current count = 0
        - Reservation list"]
        
        subgraph "Counter State"
            direction LR
            State["Track Resources:
            - Total capacity
            - Available slots
            - Active reservations"]
            
            Limits["Boundary Checks:
            0 ≤ count ≤ max"]
        end
        
        subgraph "Core Operations"
            Reserve["Reserve Resource:
            1. Check availability
            2. Decrement counter
            3. Track reservation"]
            
            Release["Release Resource:
            1. Validate reservation
            2. Increment counter
            3. Update tracking"]
            
            Monitor["Resource Monitor:
            1. Current usage
            2. Peak tracking
            3. Usage patterns"]
        end
    end

    Init --> State
    State --> Limits
    
    Limits --> Action{"Resource
    Request"}
    
    Action -->|"Reserve"| Check{"Available
    Capacity?"}
    Check -->|Yes| Reserve
    Check -->|No| Deny["Resource
    Unavailable"]
    
    Action -->|"Release"| Release
    
    Reserve --> Monitor
    Release --> Monitor
    Monitor --> State
    
    Monitor --> End([End])
    Deny --> End
```

Algorithm explanation:
1. Resource Management:
   - Atomic counter operations
   - Bounds checking (0 to max)
   - Reservation tracking
2. Core Operations:
   - Reserve: O(1) with validation
   - Release: O(1) with tracking
   - Monitor: Continuous state check
3. Safety Features:
   - Prevent overflow/underflow
   - Track invalid operations
   - Monitor usage patterns

Notes

- Thread-safe operations needed for concurrent access
- Can be extended with reservation timeouts