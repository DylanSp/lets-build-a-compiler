/*
    Runs tests on the compiler.



*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "compiler.hh"


int main() {
    
    std::string compiled_file = "test/src/compiled_out.cc";
    std::string compiled_exec = "test/bin/test_exec";
    
    ds_compiler::Compiler my_compiler;
    std::ofstream compiled_ofs (compiled_file, std::ofstream::out);
    
    my_compiler.compile_full(std::cin, compiled_ofs);
    
    std::string compile_command = "g++ " + compiled_file + " -o " + compiled_exec;
    std::cout << "Compiling: " + compile_command + '\n';
    int compile_out = std::system(compile_command.c_str());
    compiled_ofs.close();
    
    if (compile_out != 0) {
        std::cout << "Compilation failed.\n";
        return 0;
    }
    
    std::cout << "Compiled successfully!";
    std::cout << "Executing: " + compiled_exec + '\n';
    int exec_out = std::system(compiled_exec.c_str());
    if (exec_out != 0) {
        std::cout << "Execution of the compiled program failed.\n";
    } else {
        std::cout << "Compiled program executed successfully!\n";
    }
    
    
    
    
    return 0;
}

