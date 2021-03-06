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
    ds_compiler::Compiler my_compiler;
    
    
    std::string input_line = "";
    while (std::cout << PROMPT                      
            && std::getline(std::cin, input_line)) {
                
        if (input_line.at(0) == QUIT_CHAR) {
            break;
        }
        
        //std::cout << "DEBUG: " << "input_line contents = " << input_line << '\n';
        //std::cout << "DEBUG: " << "compiler_input contents = " << compiler_input.str() << '\n';
        
        my_compiler.compile_intermediate(input_line);
    }
        
    
    return 0;
}

