# function_call_stack.c

Description

Demonstrates how a function call stack works: stack frames, parameters, local variables, return addresses, and simulated recursion.

Features

- Push/pop function frames
- Add parameters and local variables
- Simulate recursion (factorial example)
- Display current frame and entire call stack

Compile (Windows PowerShell)

```powershell
gcc -o function_call_stack.exe function_call_stack.c
.\function_call_stack.exe
```

Usage

Run the program and use the interactive menu to push/pop calls, add variables, simulate recursion, and observe stack behavior.

## Code flow (Mermaid flowchart)

```mermaid
flowchart TD
  Start([Start]) --> Init[Create empty FunctionCallStack]
  Init --> Menu[Show menu / Read choice]
  Menu -->|Push| Push[Create StackFrame / Add params & locals]
  Menu -->|Pop| Pop[Remove top frame / Free variables]
  Menu -->|Simulate factorial| Simulate[Push frames recursively / Display]
  Push --> Menu
  Pop --> Menu
  Simulate --> Menu
  Menu --> Exit[Exit / Free stack]
  Exit --> End([End])
```

Notes

- This program is educational; it simulates stack frames in C structures rather than the CPU call stack.

## Function Call Stack Algorithm

```mermaid
flowchart TD
    A([Start]) --> B[Empty Stack]
    B --> C{Operation?}
    C -->|Function Call| D[Create New Frame]
    D --> E[Push to Stack]
    C -->|Return| F{Stack Empty?}
    F -->|Yes| G[/Error/]
    F -->|No| H[Pop Frame]
    H --> I[Get Return Address]
    E --> C
    I --> C
    G --> K([End])
    C -->|Exit| K
    
    subgraph "Local Variable Management"
        Menu -->|3| AddVar[/Input variable name,\ndata type/]
        AddVar --> FindFrame["frame = stack.top"]
        FindFrame --> AllocVar["var = malloc(Var)
        var.name = name
        var.type = type"]
        AllocVar --> LinkVar["var.next = frame.vars
        frame.vars = var"]
    end
    
    Menu -->|4| Exit([End])
```

Algorithm explanation:
1. Stack Frame Management:
   - Push: Create new frame → link to stack top
   - Pop: Get top frame → save return address → free
2. Local Variables:
   - Add to current frame using linked list
   - Automatically freed when frame is popped
3. Tracking:
   - Current frame count
   - Return address for each frame
   - Local variables per frame
        AllocFrame --> SetInfo[Set function name, ID, return address]
        SetInfo --> LinkFrame[Link frame to stack top]
        LinkFrame --> UpdateStack[Increment frame count & execution level]
    end
    
    subgraph "Pop Function Call"
        Pop([Return]) --> GetTop[Get top frame]
        GetTop --> SaveRet[Save return value/address]
        SaveRet --> UnlinkFrame[Unlink from stack]
        UnlinkFrame --> CleanVars[Free local variables]
        CleanVars --> DecLevel[Decrement execution level]
    end
    
    subgraph "Variable Management"
        AddVar([Add variable]) --> AllocVar[Allocate memory for variable]
        AllocVar --> FindScope[Find current frame]
        FindScope --> LinkVar[Link to frame's variable list]
    end
```
