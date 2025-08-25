# Browser Navigator Simulator  

This project simulates the behavior of a tabbed web browser with navigation functionality such as opening/closing tabs, navigating between them, and managing browsing history.  

## Features  

### 1. `NEW_TAB`  
- Creates a new tab and appends it to the browser.  

### 2. `CLOSE`  
- Closes the current tab (unless it has ID `0`, in which case `403 Forbidden` is displayed).  
- Clears the **backward** and **forward** stacks of that tab.  
- Removes the tab from the browser.  

### 3. `OPEN <ID>`  
- Switches focus to the tab with the specified ID.  
- If no such tab exists, displays `403 Forbidden`.  

### 4. `NEXT`  
- Moves to the next tab in the list.  
- If the next tab is the sentinel, wraps around to the tab after the sentinel.  

### 5. `PREV`  
- Moves to the previous tab in the list (similar to `NEXT`, but backwards).  

### 6. `PAGE <ID>`  
- Opens the page with the given ID in the current tab.  
- If the page does not exist, displays `403 Forbidden` and skips the rest of the command.  
- If found:  
  - Pushes the current page onto the **backward stack**.  
  - Clears the **forward stack**.  
  - Sets the requested page as the current one.  

### 7. `PRINT_HISTORY <ID>`  
- Prints the browsing history of the tab with the given ID:  
  1. Backward stack (recursively).  
  2. Current page.  
  3. Forward stack (recursively).  
- If the tab does not exist, displays `403 Forbidden`.  

### 8. `BACKWARD`  
- Navigates back in history.  
- If the backward stack is empty, displays `403 Forbidden`.  
- Otherwise:  
  - Pushes the current page onto the forward stack.  
  - Replaces the current page with the top of the backward stack.  

### 9. `FORWARD`  
- Navigates forward in history (symmetric to `BACKWARD`).  

### 10. `PRINT`  
- Prints the list of tabs starting from the current one.  
- Displays the description of the current page.  

## Memory Management  
At program termination, all memory is released:  
- Pages vector  
- Page descriptions  
- Browser tabs  
- Navigation stacks 
