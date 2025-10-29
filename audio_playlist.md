# audio_playlist.c

Description

A simple audio playlist simulator demonstrating basic list operations (add, remove, display) and file I/O concepts in C.

Features

- Manage a playlist (add/remove tracks)
- Save/load playlist to/from file
- Demonstrates linked list usage (or array usage depending on implementation)

Compile (Windows PowerShell)

```powershell
gcc -o audio_playlist.exe audio_playlist.c
.\audio_playlist.exe
```

Usage

Run the compiled executable and follow the menu prompts. Example: add tracks, list them, save to disk, exit.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Initialize playlist]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Add| AddTrack[Read track info / Add to list]
  Menu -->|Remove| RemoveTrack[Select track / Remove]
  Menu -->|Save/Load| SaveLoad[File I/O]
  Menu -->|Exit| Exit[Exit]
  AddTrack --> Menu
  RemoveTrack --> Menu
  SaveLoad --> Menu
  Exit --> End([End])
```

Notes

- Replace with your program-specific details if behavior differs.
