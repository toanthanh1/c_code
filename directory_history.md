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

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Init history stacks (back, forward)]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Navigate| Navigate[Push current to back; clear forward]
  Menu -->|Back| Back[Pop from back -> push to forward -> change current]
  Menu -->|Forward| Forward[Pop from forward -> push to back -> change current]
  Navigate --> Menu
  Back --> Menu
  Forward --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- You can extend to persist history to a file.
