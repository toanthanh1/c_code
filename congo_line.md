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

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Initialize empty queue]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Enqueue| Enq[Add participant]
  Menu -->|Dequeue| Deq[Remove front participant]
  Menu -->|Display| Disp[Print queue]
  Enq --> Menu
  Deq --> Menu
  Disp --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- Update commands if your program accepts arguments instead of interactive menu.
