# scavenger_hunt.c

Description

Simulates a scavenger hunt game: players search for items on a map or list; demonstrates search, state tracking, and scoring.

Features

- Place items in locations
- Players search and collect items
- Track scores and completion

Compile (Windows PowerShell)

```powershell
gcc -o scavenger_hunt.exe scavenger_hunt.c
.\scavenger_hunt.exe
```

Usage

Run and use menu to start game, move players, collect items, and view scores.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Initialize map and items]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Start game| StartGame[Place players]
  Menu -->|Move| Move[Change player position -> check for item]
  Move -->|Item found| Collect[Collect item -> update score]
  Collect --> Menu
  Move --> Menu
  StartGame --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- Include game-specific rules if implementation differs.
