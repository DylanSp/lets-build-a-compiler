/* 
    The compiler itself.
    
*/

#ifndef COMPILER_HH
#define COMPILER_HH

#include <iostream>
#include "cradle.cc"


namespace ds_compiler {

class Compiler {
  
public:
    Compiler();
    
    //"main" methods, compile from is to os
    void compile_intermediate(std::istream &is, std::ostream &os) const;  //compiles a single line of input
    void compile_full (std::istream &is, std::ostream &os) const;         //compiles a full C++ program


private:
    void compile_start(std::istream &is, std::ostream &os) const;
    void compile_end(std::istream &is, std::ostream &os) const;
    
};

} //end namespace 

#endif