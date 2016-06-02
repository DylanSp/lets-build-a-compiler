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
    Compiler(std::istream *new_is, std::ostream *new_os);
    
    //"main" methods, compile from is to os
    void compile_intermediate() const;  //compiles a single line of input
    void compile_full () const;         //compiles a full C++ program
    
    void expression() const;
    
    void set_is(std::istream *new_is);
    void set_os(std::ostream *new_os);


private:
    static const size_t NUM_REGISTERS;    //number of registers available to the compiled code
    static const char ERR_CHAR;

    void compile_start() const;
    void compile_end() const;
    
    
    std::istream& is() const;
    std::ostream& os() const;
    mutable std::istream *m_is;
    mutable std::ostream *m_os;

    
};

} //end namespace 

#endif