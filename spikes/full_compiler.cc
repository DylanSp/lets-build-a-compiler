/* 
    Driver program; runs compiler to generate a full program from one line

*/

#include <iostream>
#include <fstream>
#include <string>
#include "compiler.hh"

int main () {
    
    const std::string PROJ_ROOT("/home/ubuntu/workspace/");
    std::string class_name("TestClass");
    std::string output_filename("test/src/" + class_name + ".cc");
    
    std::ofstream ofs(output_filename, std::ofstream::out);

    ds_compiler::Compiler my_compiler(ofs);

    const std::string PROMPT("Enter the line to be compiled:\n");
    std::string input_line = "";
    
    std::cout << PROMPT;
    std::getline(std::cin, input_line);
    
    std::vector<std::string> program;
    program.push_back(input_line);
    
    //wrap in try/catch
    my_compiler.compile_full(program, class_name);
        
    return 0;
}

