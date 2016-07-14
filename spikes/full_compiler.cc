/* 
    Driver program; runs compiler to generate a full program from one line

*/

#include <iostream>
#include <fstream>
#include <string>
#include "compiler.hh"

void generate_main (const std::string class_name, std::ofstream& ofs) {
    std::string object_name("sample_object");
    /*
    int main () {
    
    SampleClass sc;
    sc.run();
    sc.dump();
    
    return 0;
}
    
    */
    
    ofs << "int main () {" << '\n';
    ofs << class_name << " " << object_name << ";" << '\n';
    ofs << object_name << ".run();" << '\n';
    ofs << object_name << ".dump();" << '\n';
    ofs << "return 0; }" << '\n';
}

int main () {
    
    const std::string PROJ_ROOT("/home/ubuntu/workspace/");
    std::string class_name("SampleClass");
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
    try {
        my_compiler.compile_full(program, class_name);
        generate_main(class_name, ofs);
        std::cout << class_name << " compiled successfully to " << output_filename << "." << '\n';
        std::cout << "Run 'make sample' to build; bin/sample to execute." << '\n';
    } catch (std::exception &ex) {
        std::cerr << ex.what() << '\n';
    }
    
        
    return 0;
}

