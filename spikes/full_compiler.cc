/* 
    Driver program; runs compiler to generate a full program from one line

*/

#include <iostream>
#include <string>
#include "compiler.hh"

int main () {
    
    //TODO - output to a file in src/ (right now, outputs to cout)
    //just specify filename in source code here
    //

    const std::string PROMPT = std::string("Enter the line to be compiled:\n");
    ds_compiler::Compiler my_compiler;
    std::string input_line = "";
    
    std::cout << PROMPT;
    std::getline(std::cin, input_line);
    
    std::vector<std::string> program;
    program.push_back(input_line);
    
    //wrap in try/catch
    my_compiler.compile_full(program, "testclass");
        
    return 0;
}

