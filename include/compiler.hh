/* 
    The compiler itself.
    
*/

#ifndef COMPILER_HH
#define COMPILER_HH

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>

namespace ds_compiler {

class Compiler {
  
public:
    Compiler();
    Compiler(std::istream *new_is, std::ostream *new_os);
    
    //"main" methods, compile from is to os
    void compile_intermediate(const std::string input_line);  //compiles a single line of input
    void compile_full (const std::vector<std::string> source);         //compiles a full C++ program
    

    //compiling (parts of) lines 
    void assignment();
    void expression();
    void term();
    void factor();
    void ident();
    void add();
    void subtract();
    void multiply();
    void divide();
    
    void set_is(std::istream *new_is);
    void set_os(std::ostream *new_os);


private:
    static const size_t NUM_REGISTERS;          //number of registers available to the compiled code
    static const char ERR_CHAR;
    static const std::unordered_set<char> ADD_OPS;
    static const std::unordered_set<char> MULT_OPS;

    void compile_start() const;
    void compile_end() const;
    
    //cradle methods
    void report_error(const std::string err) const;
    void abort(const std::string err) const;
    void expected(const std::string expect) const;
    void expected(const char c) const;
    void match(const char c);
    char get_name ();
    char get_num ();
    void emit (std::string s) const;
    void emit_line (std::string s) const;
    
    static bool is_in(const char elem, const std::unordered_set<char> us);
    
    //helper members to allow stream syntax with pointers to streams
    std::istream& is() const;
    std::ostream& os() const;
    mutable std::istream *m_is;
    mutable std::ostream *m_os;
    
    std::stringstream m_input_stream;

    
};

} //end namespace 

#endif