# elimination_game.c

Description

Simulates an elimination game (Josephus problem or similar) removing players in rounds until one remains.

Features

- Simulate elimination order
- Parameterize step size and list size

Compile (Windows PowerShell)

```powershell
gcc -o elimination_game.exe elimination_game.c
.\elimination_game.exe
```

Usage

Provide number of players and step size; program prints elimination order and winner.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Read[Read n (players), k (step)]
  Read --> Create[Create circular list of players]
  Create --> Loop[Loop until one remains]
  Loop --> Move[Move k-1 steps]
  Move --> Remove[Remove current player]
  Remove --> Loop
  Loop --> Winner[Print winner]
  Winner --> End([End])
```

Notes

- For large n, consider O(n) simulation vs mathematical solution.
