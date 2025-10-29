# token_system.c

Description

Implements a token issuance/validation system (e.g., simple authentication tokens or resource tokens) demonstrating string handling and lookup.

Features

- Issue tokens
- Validate tokens
- Revoke tokens

Compile (Windows PowerShell)

```powershell
gcc -o token_system.exe token_system.c
.\token_system.exe
```

Usage

Run and use menu to issue, validate, and revoke tokens.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Init token store (list/hash)]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Issue| Issue[Generate token -> add to store]
  Menu -->|Validate| Validate[Lookup token -> report valid/invalid]
  Menu -->|Revoke| Revoke[Remove token]
  Issue --> Menu
  Validate --> Menu
  Revoke --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- For production, use secure random and proper storage; this is a demo.
