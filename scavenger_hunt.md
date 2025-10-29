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

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core Game System"
        Init["Initialize:
        - Game map
        - Item locations
        - Player states"]
        
        subgraph "Game Components"
            direction LR
            Map["Game Map:
            - Grid structure
            - Location states
            - Item placement"]
            
            Items["Item System:
            - Item types
            - Locations
            - Collection rules"]
            
            Players["Player Tracking:
            - Positions
            - Inventories
            - Scores"]
        end
        
        subgraph "Core Mechanics"
            Search["Search Operation:
            1. Check location
            2. Find items
            3. Update state"]
            
            Collect["Item Collection:
            1. Validate find
            2. Update inventory
            3. Calculate score"]
            
            Progress["Game Progress:
            1. Track finds
            2. Update rankings
            3. Check completion"]
        end
    end

    Init --> Map
    Map --> Items
    Items --> Players
    
    Players --> Action{"Player
    Action"}
    
    Action -->|"Move"| Location{"Valid
    Location?"}
    Location -->|Yes| Search
    Location -->|No| Invalid["Cannot
    move there"]
    
    Search --> Found{"Item
    Present?"}
    Found -->|Yes| Collect
    Found -->|No| Continue["Next
    Move"]
    
    Collect --> Progress
    Continue --> Progress
    
    Progress --> Complete{"Game
    Complete?"}
    Complete -->|Yes| End([End])
    Complete -->|No| Action

    Invalid --> Action
```

Algorithm explanation:
1. Game Setup:
   - Initialize map grid
   - Place items randomly
   - Set player starting positions
2. Core Mechanics:
   - Movement validation
   - Item discovery
   - Score calculation
3. Game Progress:
   - Track item collection
   - Update player standings
   - Check win conditions

Notes

- Map can be grid or location list
- Items can have different values/rules