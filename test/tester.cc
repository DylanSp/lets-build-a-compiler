/*
    Runs tests on the compiler.



*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "compiler.hh"


int main() {
    
    std::string input_file = "test/src/parens.test";
    std::string compiled_file = "test/src/compiled_out.cc";
    std::string compiled_exec = "test/bin/test_exec";
    
    
    std::ifstream test_input (input_file, std::ifstream::in);
    std::ofstream compiled_ofs (compiled_file, std::ofstream::out);
    
    ds_compiler::Compiler my_compiler(&test_input,&compiled_ofs);
    my_compiler.compile_full();
    compiled_ofs.close();
    test_input.close();
    
    std::string compile_command = "g++ -std=c++11 " + compiled_file + " -o " + compiled_exec;
    std::cout << "Compiling with g++: " + compile_command + '\n';
    int compile_out = std::system(compile_command.c_str());
    
    
    if (compile_out != 0) {
        std::cout << "Compilation failed.\n";
        return 0;
    }
    
    std::cout << "Compiled successfully!\n";
    std::cout << "Executing: " + compiled_exec + '\n';
    int exec_out = std::system(compiled_exec.c_str());
    if (exec_out != 0) {
        std::cout << "Execution of the compiled program failed.\n";
    } else {
        std::cout << "Compiled program executed successfully!\n";
    }
    
    
    
    
    return 0;
}

