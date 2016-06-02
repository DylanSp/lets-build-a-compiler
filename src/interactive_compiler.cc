/* 
    Driver program; runs compiler in an interactive mode.

*/

#include <iostream>
#include <sstream>
#include <string>
#include "compiler.hh"

int main () {
    

    const char QUIT_CHAR = '$';
    const std::string PROMPT = std::string("Enter a line to be compiled ('") + QUIT_CHAR + "' to quit):\n";
    std::stringstream compiler_input(std::ios::in|std::ios::out);    
    ds_compiler::Compiler my_compiler(&compiler_input, &std::cout);
    
    
    std::string input_line = "";
    while (std::cout << PROMPT                      //TODO - figure out why this needs two line breaks on first call.
            && std::getline(std::cin, input_line)
            && std::cin.peek() != QUIT_CHAR) {
        compiler_input << input_line;
        my_compiler.compile_intermediate();
    }
        
    
    return 0;
}

