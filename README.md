# Just a simple compiler for my Computer Programming Languages course  
Scans, parses, then interprets and runs a lua file  
This project only works with an extremely simplified version of lua  

#### Project Components:  
`1.` Scanner  
`2.` Parser  
`3.` Interpreter  

#### Progress:  
`1.` Done  
`2.` Done  
`3.` Done  

# Notes  
To compile, be sure to include all files in the compile line:  
g++ -std=c++11 Token.cpp Integer.cpp scanner.cpp ParseNode.cpp parser.cpp interpreter.cpp main.cpp -o runme  

A number of additional commands have been added to the main program. You may now enter `-parse` or `-scan` to run only the parser or scanner. You may also enter `-log` to enable the parser's full real-time parse tree.  

##### WARNING:  
I don't know what's up with windows, but this program **will not function** with the current version of gcc. I can only assume this is because the windows version of gcc is currently 5.1.0 while the one on my linux machine is 5.4.0 . Regardless, what ever the cause, the effect is that the scanner's `peekToken` function doesn't reset the inFile's position. This is a fairly critical component and breaks the whole compiler.  

# Report:  
## Original Assignment  
Develop a complete interpreter that works in conjuction with a scanner and parser.  

## Solution  
While developing the parser portion of this project, I developed it in such a way that it would be fairly easily modified to include a parser. The interpreter, in essence, is just a collection of a few functions that execute things. These functions are called by the parser. In addition to this, the interpreter handles all the data stored in variables.  

The hardest problem in this phase was thinking through how to correctly allow nested if statements and loops to function properly. This was achieved by using vector lists in the interpreter to keep track of which portion an if statement should run (`ifStack`) and where the beginning of each loop was (`loopStack`).  

## Example Output  
Sample output can be found in the [output.txt](https://github.com/DariusMiu/Lua-Compiler-Project/blob/master/output.txt) file (from running the test.lua file).  
