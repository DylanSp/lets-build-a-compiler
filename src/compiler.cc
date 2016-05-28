/*
    Implementation of the Compiler class.


*/

#include "compiler.hh"

namespace ds_compiler {
    
//constructors



void Compiler::compile_intermediate (std::istream &is, std::ostream &os) const {
    os << "Test\n";
    
    //note - emitted instructions manipulate stack, array of registers
}
    
void Compiler::compile_full (std::istream &is, std::ostream &os) const {
    //TODO
    compile_start(is, os);
    
    compile_end(is, os);
    
}

void Compiler::compile_start (std::istream &is, std::ostream &os) const {
    //TODO
    
    
    //emit lines for necessary includes
    //emit lines for int main() {
    //create a stack and array (?) of registers
    
}

void Compiler::compile_end (std::istream &is, std::ostream &os) const {
    //TODO
    
    //emit line for closing main
    
}
    
    
} //end namespace