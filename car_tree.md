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

## Core Algorithm (Mermaid flowchart)

```mermaid
flowchart TD
    Begin([Begin]) --> Init

    subgraph "Core BST Operations"
        Init["Initialize Empty Tree:
        root = NULL"]
        
        subgraph "Node Structure"
            direction LR
            Node["Car Node:
            - Model/Key
            - Details
            - Left child
            - Right child"]
        end
        
        subgraph "Tree Operations"
            Insert["Binary Insert:
            1. Compare keys
            2. Choose subtree
            3. Maintain BST property"]
            
            Search["Binary Search:
            1. Start at root
            2. Compare and branch
            3. Recurse until found"]
            
            Traverse["Tree Traversal:
            Inorder (sorted):
            - Left subtree
            - Current node
            - Right subtree"]
        end
    end

    Init --> Node
    Node --> Compare{"Compare Keys"}
    
    Compare -->|"Key < Node"| Left["Go Left:
    Smaller values"]
    Compare -->|"Key > Node"| Right["Go Right:
    Larger values"]
    
    Left --> Insert
    Right --> Insert
    
    Insert --> BST{"BST Property:
    Left < Node < Right"}
    
    BST --> Search
    Search --> Traverse
    
    Traverse --> End([End])
```

Algorithm explanation:
1. Binary Search Tree:
   - Each node has up to 2 children
   - Left subtree contains smaller keys
   - Right subtree contains larger keys
2. Time Complexity:
   - Insert: O(log n) average
   - Search: O(log n) average
   - Traversal: O(n)
3. Optimization:
   - Balance tree for best performance
   - Handle duplicates appropriately

Notes

- Tree balance affects performance
- Inorder traversal gives sorted output