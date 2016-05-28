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
    Compiler(std::istream *is, std::ostream *os);

private:

    std::istream *input;
    std::ostream *output;
    
};

} //end namespace 

#endif