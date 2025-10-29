# car_tree.c

Description

Implements a tree (or binary tree) structure representing car data (e.g., models, categories) and supports traversal/search operations.

Features

- Insert car nodes
- Search by key
- Traversals (inorder/preorder/postorder)

Compile (Windows PowerShell)

```powershell
gcc -o car_tree.exe car_tree.c
.\car_tree.exe
```

Usage

Run the program and use the menu to insert/search/traverse nodes.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Initialize root = NULL]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Insert| InsertNode[Create node / Insert into tree]
  Menu -->|Search| Search[Traverse to find key]
  Menu -->|Traverse| Traverse[Perform traversal]
  InsertNode --> Menu
  Search --> Menu
  Traverse --> Menu
  Menu --> Exit[Exit]
  Exit --> End([End])
```

Notes

- If your implementation is different (AVL, BST), update README accordingly.
