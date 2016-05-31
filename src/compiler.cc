/*
    Implementation of the Compiler class.


*/

#include "compiler.hh"
#include <string>

namespace ds_compiler {
    
size_t Compiler::NUM_REGISTERS = 8;
    
//constructors
Compiler::Compiler () {
    
}


void Compiler::compile_intermediate (std::istream &is, std::ostream &os) const {

}
    
void Compiler::compile_full (std::istream &is, std::ostream &os) const {
    
    compile_start(is, os);
    compile_intermediate(is, os);
    compile_end(is, os);
    
}

void Compiler::compile_start (std::istream &is, std::ostream &os) const {
    
    //emit lines for necessary includes
    emit_line("#include <stack>", os);
    emit_line("#include <vector", os);
    
    //emit lines for int main() {
    emit_line("int main () {", os);
    
    //create a stack and vector for registers
    std::string stack_init = "std::stack<int> cpu_stack;";
    std::string registers_init = "std::vector<int> cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0);";
    
    emit_line(stack_init, os);
    emit_line(registers_init, os);
    
}

void Compiler::compile_end (std::istream &is, std::ostream &os) const {
    
    //emit line for closing main
    emit_line("}", os);
    
}
    
    
} //end namespace