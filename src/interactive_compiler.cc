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
        
        //std::cout << "DEBUG: " << "input_line contents = " << input_line << '\n';
        //std::cout << "DEBUG: " << "compiler_input contents = " << compiler_input.str() << '\n';
        
        my_compiler.compile_intermediate();
        compiler_input.str(""); //clear stringstream
        compiler_input.clear();
    }
        
    
    return 0;
}

