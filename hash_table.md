# hash_table.c

Description

Implements a hash table with insert/search/delete operations and collision handling (chaining or open addressing).

Features

- Insert key/value
- Search by key
- Delete key
- Collision resolution

Compile (Windows PowerShell)

```powershell
gcc -o hash_table.exe hash_table.c
.\hash_table.exe
```

Usage

Run and use menu-driven operations to test the hash table.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Init table (array of buckets)]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Insert| Insert[Compute hash -> place in bucket]
  Menu -->|Search| Search[Compute hash -> search chain/probe]
  Menu -->|Delete| Delete[Locate and remove]
  Insert --> Menu
  Search --> Menu
  Delete --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- Mention the chosen hash function and table size if fixed in your implementation.
