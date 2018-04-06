## Just a simple compiler for my Computer Programming Languages course  
Scans, parses, then interprets and runs a lua file  
This project only works with an extremely simplified version of lua  

##### Project Components:  
`1.` Scanner  
`2.` Parser  
`3.` Interpreter  

##### Progress:  
`1.` Done  
`2.` In Progress  
`3.` Pending  

## Notes:  
To compile, be sure to include all files in the compile line:  
g++ Token.cpp scanner.cpp parser.cpp main.cpp -o scanner  

When run, the program will automatically run against test.lua. To run on a different file, simply call it from command line like so:  
`./main mow.lua`  