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
    Compiler(std::ostream& output = std::cout);
    
    //"main" methods, compile from is to os
    void compile_intermediate(const std::string input_line);  //compiles a single line of input
    void compile_full (const std::vector<std::string> source, const std::string class_name);         //compiles a full C++ program
    
private:
    static const size_t NUM_REGISTERS;          //number of registers available to the compiled code
    static const char ERR_CHAR;
    static const std::unordered_set<char> ADD_OPS;
    static const std::unordered_set<char> MULT_OPS;
    static const char END_CHAR;
    static const std::unordered_set<char> BLOCK_ENDS;
    static const char IF_CHAR;
    static const char ELSE_CHAR;

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
    
    //parsing methods
    void start_symbol();
    void program();
    void block();
    void parse_if();
    void condition();
    void branch_on_cond(const std::string label);
    void branch_on_not_cond(const std::string label);
    void jump(const std::string label);
    void other();
    
    //label handling
    size_t label_count;
    std::string new_label();
    void post_label(const std::string label) const;

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
    
    std::stringstream m_input_stream;
    std::ostream& m_output_stream;
    
};

} //end namespace 

#endif