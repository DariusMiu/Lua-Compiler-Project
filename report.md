# Report:  
## Original Assignment  
Develop a complete interpreter that works in conjuction with a scanner and parser.  

## Solution  
While developing the parser portion of this project, I developed it in such a way that it would be fairly easily modified to include a parser. The interpreter, in essence, is just a collection of a few functions that execute things. These functions are called by the parser. In addition to this, the interpreter handles all the data stored in variables.  

The hardest problem in this phase was thinking through how to correctly allow nested if statements and loops to function properly. This was achieved by using vector lists in the interpreter to keep track of which portion an if statement should run (`ifStack`) and where the beginning of each loop was (`loopStack`). Running the mow.lua file shows that nested loops are fully functional.  

## Example Output  
Sample output can be found in the [output.txt](https://github.com/DariusMiu/Lua-Compiler-Project/blob/master/output.txt) file (from running the test.lua file).  
