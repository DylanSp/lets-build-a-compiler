/*
    Implementation of the Compiler class.


*/

#include "compiler.hh"
#include <string>

namespace ds_compiler {
    
const size_t Compiler::NUM_REGISTERS = 8;
const char Compiler::ERR_CHAR = '\0';
    
//constructors
Compiler::Compiler () 
    : m_is (&std::cin), m_os (&std::cout)
{
    
}


Compiler::Compiler (std::istream *new_is, std::ostream *new_os) 
    : m_is (new_is), m_os (new_os) 
{
        
}


void Compiler::compile_intermediate () const {

}
    
void Compiler::compile_full () const {
    
    compile_start();
    compile_intermediate();
    compile_end();
    
}

void Compiler::compile_start () const {
    
    //emit lines for necessary includes
    emit_line("#include <stack>", this->os());
    emit_line("#include <vector>", this->os());
    
    //emit lines for int main() {
    emit_line("int main () {", os());
    
    //create a stack and vector for registers
    std::string stack_init = "std::stack<int> cpu_stack;";
    std::string registers_init = "std::vector<int> cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0);";
    
    emit_line(stack_init, os());
    emit_line(registers_init, os());
    
}

void Compiler::compile_end () const {
    
    //emit lines for closing main
    emit_line("return 0;", os());
    emit_line("}", os());
    
}
    
void Compiler::parse_expression () {
    
}   

void Compiler::set_is (std::istream *new_is) {
    m_is = new_is;
}

void Compiler::set_os (std::ostream *new_os) {
    m_os = new_os;
}

std::istream& Compiler::is() const {
    return *m_is;
}

std::ostream& Compiler::os() const {
    return *m_os;
}
    
} //end namespace