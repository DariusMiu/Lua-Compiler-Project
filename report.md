## Original Assignment  
Develop a complete parser that executes in conjunction with the scanner.  

## Solution  
First, I began by modifying the code I had already developed for the scanner. Previously, the scanner was only a single file that contained the Token class and was compiled and ran by itself. Now, the scanner is a standalone file, as is Token, and both have necessary header files. In addition, a `peekToken` method was added to the scanner. This way, functions like `<statement>` can determine which statement it is, without removing the first token (which may need to be used by the statement itself!)  

After this was accomplished, work could begin on the parser in earnest. A few different methods were tried, but the one that was finally implemented was a very object-oriented approach. Each grammer element (ex: `<block>`) has its own method. This way, each method may be tight and focused (every method is under 35 lines of code) and can be called as required. I foundthe result of this approach to be quite beautiful.  

Every method does exactly what is stated in the grammar, whith the exception of the `<block>` method, which adds a newline before a new recursive `<block>` for easier reading (it's still very difficult to read, but this made it much easier)  

## Example Output  
Sample output can be found in the [output.txt](https://github.com/DariusMiu/Lua-Compiler-Project/blob/master/output.txt) file (from parsing the test.lua file).  
