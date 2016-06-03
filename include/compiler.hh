/* 
    The compiler itself.
    
*/

#ifndef COMPILER_HH
#define COMPILER_HH

#include <iostream>
#include <string>
#include <cctype> //character comparison functions


namespace ds_compiler {

class Compiler {
  
public:
    Compiler();
    Compiler(std::istream *new_is, std::ostream *new_os);
    
    //"main" methods, compile from is to os
    void compile_intermediate() const;  //compiles a single line of input
    void compile_full () const;         //compiles a full C++ program
    

    //compiling (parts of) lines 
    void expression() const;
    void term() const;
    void add() const;
    void subtract() const;
    
    void set_is(std::istream *new_is);
    void set_os(std::ostream *new_os);


private:
    static const size_t NUM_REGISTERS;    //number of registers available to the compiled code
    static const char ERR_CHAR;

    void compile_start() const;
    void compile_end() const;
    
    //cradle methods
    void report_error(const std::string err) const;
    void abort(const std::string err) const;
    void expected(const std::string expect) const;
    void expected(const char c) const;
    void match(const char c) const;
    char get_name () const;
    char get_num () const;
    void emit (std::string s) const;
    void emit_line (std::string s) const;
    
    
    //helper members to allow stream syntax with pointers to streams
    std::istream& is() const;
    std::ostream& os() const;
    mutable std::istream *m_is;
    mutable std::ostream *m_os;

    
};

} //end namespace 

#endif