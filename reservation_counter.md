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

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Initialize counter (max capacity)]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Reserve| Reserve{Available > 0?}
  Reserve -->|Yes| Decrement[Decrement count]
  Reserve -->|No| Full[Report full / cannot reserve]
  Menu -->|Release| Release[Increment (up to max)]
  Decrement --> Menu
  Release --> Menu
  Full --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- If the program uses file persistence, mention the file name.
