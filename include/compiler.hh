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
#include <set>
#include <map>

namespace ds_compiler {
    
class Compiler {
  
public:
    using Symbol = std::string;
    enum SymbolType {IF_SYM, ELSE_SYM, ENDIF_SYM, END_SYM, IDENT, NUMBER, OPERATOR_SYM, FAILURE };
    static const std::map<std::string, SymbolType> SymbolTypeNames;

    struct Token {
        SymbolType type;
        std::string value;
    }; 


    Compiler(std::ostream& output = std::cout);
    
    //"main" methods, compile from is to os
    void compile_intermediate(const std::string input_line);  //compiles a single line of input
    void compile_full (const std::string source, const std::string class_name);         //compiles a full C++ program
    
    static const size_t NUM_REGISTERS;          //number of registers available to the compiled code
                                                //public so test generation code can reference it
    
private:

    static const std::string ERR_STRING;
    static const std::unordered_set<char> ADD_OPS;
    static const std::unordered_set<char> MULT_OPS;
    static const char TRUE_CHAR;
    static const char FALSE_CHAR;
    static const std::unordered_set<char> BOOLEAN_LITERALS;
    static const std::unordered_set<char> WHITESPACE;
    static const std::unordered_set<char> OPERATORS;
    static const std::map<Symbol, std::string> KEYWORDS;

    void compile_start(const std::string class_name) const;
    void compile_end() const;
    
    //code generation methods
    void add_includes() const;
    void define_member_variables() const;
    void define_constructor(const std::string class_name) const;
    void define_cpu_pop() const;
    void define_getters() const;
    void define_is_stack_empty() const;
    void define_dump() const;
    
    //lexing methods
    Token scan();
    Token get_op();
    
    //parsing methods
    void start_symbol();

    //boolean handling
    bool get_boolean();
    static bool is_boolean (const char c);

    void line_end();

    //cradle methods
    void report_error(const std::string err) const;
    void abort(const std::string err) const;
    void expected(const std::string expect) const;
    void expected(const char c) const;
    void skip_whitespace();
    void match(const char c);
    Token get_name ();
    Token get_num ();
    void emit (std::string s) const;
    void emit_line (std::string s) const;
    
    static bool is_in(const char elem, const std::unordered_set<char> us);
    
    std::stringstream m_input_stream;
    std::ostream& m_output_stream;
    std::set<Symbol> m_symbol_table;
    
};

} //end namespace 

#endif