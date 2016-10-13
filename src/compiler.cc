/*
    Implementation of the Compiler class.


*/

#include <cctype>       //character comparison functions
#include <stdexcept>
#include <assert.h>
#include <utility>      //std::pair
#include "compiler.hh"

namespace ds_compiler {


const std::map<std::string, Compiler::SymbolType> Compiler::SymbolTypeNames {{"IfSym", IF_SYM}, {"ElseSym", ELSE_SYM}, 
                                                                             {"EndifSym", ENDIF_SYM}, {"EndSym", END_SYM},
                                                                             {"Ident", IDENT}, {"Number", NUMBER},
                                                                             {"OperatorSym", OPERATOR_SYM}};
                                                                             
const size_t Compiler::NUM_REGISTERS = 8;
const std::string Compiler::ERR_STRING = "ERR";
const std::unordered_set<char> Compiler::ADD_OPS({'+', '-'});
const std::unordered_set<char> Compiler::MULT_OPS({'*', '/'});
const char Compiler::TRUE_CHAR = 'T';
const char Compiler::FALSE_CHAR = 'F';
const std::unordered_set<char> Compiler::BOOLEAN_LITERALS({TRUE_CHAR, FALSE_CHAR});
const std::unordered_set<char> Compiler::WHITESPACE({' ', '\t', '\r', '\n'});
const std::unordered_set<char> Compiler::OPERATORS({'+', '-', '*', '/', '<', '>', ':', '='});
const std::map<Compiler::Symbol, std::string> Compiler::KEYWORDS{{"IF", "IfSym"}, {"ELSE", "ElseSym"}, {"ENDIF", "EndifSym"}, {"END", "EndSym"}};
    
//constructors
Compiler::Compiler (std::ostream& output) 
    : m_input_stream (std::ios::in|std::ios::out), m_output_stream(output), m_symbol_table()
{
    
}



void Compiler::compile_intermediate (const std::string input_line) {
    
    //clear contents and error flags on m_input_stream
    m_input_stream.str("");
    m_input_stream.clear();
    
    m_input_stream << input_line;
    
    try {
        //start_symbol();
        
        Token token;
        do {
            token = scan();
            switch (token.type) {
                case IDENT:
                    std::cout << "Ident ";
                    break;
                case NUMBER:
                    std::cout << "Number ";
                    break;
                case OPERATOR_SYM:
                    std::cout << "Operator ";
                    break;
                case IF_SYM:
                case ELSE_SYM:
                case ENDIF_SYM:
                case END_SYM:
                    std::cout << "Keyword ";
                    break;
                default:
                    assert(false);  //should never be reached!
                    break;
            }
            std::cout << token.value << '\n';
        } while (token.type != END_SYM && m_input_stream.peek() != std::char_traits<char>::eof());
        
        
    } catch (std::exception &ex) {
        std::cerr << ex.what() << '\n';
        throw std::runtime_error("Compilation failed.\n");
    }
}
    
void Compiler::compile_full (const std::string source, const std::string class_name) {
    
    compile_start(class_name);
    compile_intermediate(source);
    compile_end();
    
}

void Compiler::compile_start (const std::string class_name) const {
    add_includes();    
   
    //begin class declaration, qualify everything as public
    emit_line("class "+ class_name + "{");
    emit_line("public:");
    
    define_member_variables();
    define_constructor(class_name);
    define_cpu_pop();
    define_getters();
    define_is_stack_empty();
    
    emit_line("void run() {");  //begin definition of run()
}

void Compiler::compile_end () const {
    //TODO - should I assert that cpu_stack is empty?
    
    emit_line("}");     //end definition of run()
    define_dump();
    emit_line("};");    //close class definition
}

void Compiler::add_includes() const {
    emit_line("#include <stack>");
    emit_line("#include <vector>");
    emit_line("#include <iostream>");
    emit_line("#include <string>");
    emit_line("#include <unordered_map>");
}

void Compiler::define_member_variables() const {
    emit_line("std::stack<int> cpu_stack;");
    emit_line("std::vector<int> cpu_registers;");
    emit_line("std::unordered_map<char, int> cpu_variables;");
}

void Compiler::define_constructor(const std::string class_name) const {
    emit_line(class_name + "() ");
    emit_line(": cpu_stack()");
    emit_line(", cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0)");
    emit_line(", cpu_variables()");
    emit_line("{}");
}

//emit definition of a function for easier stack handling
void Compiler::define_cpu_pop() const {
    emit_line("int cpu_pop() {");
    emit_line("int val = cpu_stack.top();");
    emit_line("cpu_stack.pop();");
    emit_line("return val; }");
}

void Compiler::define_getters() const {
    emit_line("int get_register(int index) {");
    emit_line("return cpu_registers.at(index);}");
    
    emit_line("int get_variable(char var_name) {");
    emit_line("return cpu_variables.at(var_name);}");
    
    //no getter for stack; stack should always be empty
}

void Compiler::define_is_stack_empty() const {
    emit_line("bool is_stack_empty() {");
    emit_line("return cpu_stack.empty();}");
}

void Compiler::define_dump() const {
    emit_line("void dump () {");
    
    emit_line("std::cout << \"Register contents\\n\";");
    emit_line("for (int i = 0; i < " + std::to_string(NUM_REGISTERS) + "; ++i)");
    emit_line("std::cout << std::string(\"Register \") << i << \": \" << cpu_registers.at(i) << '\\n';");
    
    emit_line("std::cout << \"Stack contents (top to bottom)\\n\";");
    emit_line("while (!cpu_stack.empty()) {");
    emit_line("std::cout << cpu_stack.top() << '\\n';");
    emit_line("cpu_stack.pop();}");
    
    emit_line("std::cout << \"Variable contents\\n\";");
    emit_line("for (auto i = cpu_variables.begin(); i != cpu_variables.end(); ++i)"); 
    emit_line("std::cout << \"cpu_variables[\" << i->first << \"] = \" << i->second << '\\n';");
    
    emit_line("}");
}

Compiler::Token Compiler::scan () {
    Compiler::Token token;
    char look = m_input_stream.peek();
    
    if (std::isalpha(m_input_stream.peek())) {
        token = get_name();
    } else if (std::isdigit(m_input_stream.peek())) {
        token = get_num();
    } else if (is_in(m_input_stream.peek(), OPERATORS)) {
        token = get_op();
    } else {
        token.value = m_input_stream.get();
        token.type = OPERATOR_SYM;
    }
    skip_whitespace();
    return token;
}

Compiler::Token Compiler::get_op () {
    Token op;
    op.value = "";
    char look = m_input_stream.peek();
    
    if (!is_in(look, OPERATORS)) {
        expected("Operator");
        return op; //shouldn't return; expected() will abort program
    } 
    
    while (is_in(look, OPERATORS)) {
        op.value += m_input_stream.get();
        look = m_input_stream.peek();
    }
    
    op.type = OPERATOR_SYM;
    skip_whitespace();
    return op;
    
}

void Compiler::start_symbol () {
    //std::cout << get_name();
    //std::cout << get_num();
}


//boolean handling

bool Compiler::get_boolean () {
    if (!is_boolean(m_input_stream.peek())) {
        expected("Boolean literal");    //will throw exception
    } 
    
    bool boolean_value = std::toupper(m_input_stream.peek()) == TRUE_CHAR;
    m_input_stream.get();
    return boolean_value;
}

bool Compiler::is_boolean (const char c) {
    return is_in(std::toupper(c), BOOLEAN_LITERALS);
}

//skip over line endings
void Compiler::line_end () {
    if (m_input_stream.peek() == '\r') {
        m_input_stream.get();
    }
  
    if (m_input_stream.peek() == '\n') {
        m_input_stream.get();
    }
}

//cradle methods

void Compiler::report_error(const std::string err) const {
    
    m_output_stream << '\n';
    m_output_stream << "Error: " << err << '\n';
    
}

void Compiler::abort(const std::string err) const {
    
    report_error(err);
    throw std::runtime_error("Compilation failed.\n");
}

void Compiler::expected(const std::string expect) const {
    
    abort(expect + " expected.\n");
}

//overload to handle single characters; 
//prevents having to construct a string whenever calling expected()
void Compiler::expected(const char c) const {
    expected(std::string(1, c));
}

void Compiler::skip_whitespace () {
    while (is_in(m_input_stream.peek(), WHITESPACE)) {
        m_input_stream.get();  //intentionally discard result 
    }
}


//checks if next character matches; if so, consume that character
void Compiler::match(const char c) {
    
    if (m_input_stream.peek() == c) {
        m_input_stream.get(); 
    } else {
        expected(c);
    }
}

// gets a valid identifier from input stream
Compiler::Token Compiler::get_name () {
    Token name;
    name.value = "";
    char look = m_input_stream.peek();
    
    if (!std::isalpha(m_input_stream.peek())) {
        expected("Name");
        return name; //shouldn't return; expected() will abort program
    } 
    
    //while (std::isalnum(m_input_stream.peek())) {
    while (std::isalnum(look)) {
        name.value += std::toupper(m_input_stream.get());
        look = m_input_stream.peek();
    }
    
    if (KEYWORDS.find(name.value) != KEYWORDS.end()) {
        name.type = SymbolTypeNames.at(KEYWORDS.at(name.value));
    } else {
        name.type = IDENT;
    }
    
    skip_whitespace();
    return name;
}

//gets a number
Compiler::Token Compiler::get_num () {
    Token num;
    num.value = "";
    
    if (!std::isdigit(m_input_stream.peek())) {
        expected("Integer");
        return num; //shouldn't return; expected() will abort program
    } 
    
    while (std::isdigit(m_input_stream.peek())) {
        num.value += m_input_stream.get();
    }
    
    num.type = NUMBER;
    skip_whitespace();
    return num;
}


//output a string 
void Compiler::emit (std::string s) const {
    m_output_stream << s;
}

//output a string with newline 
void Compiler::emit_line (std::string s) const {
    emit(s);
    emit("\n");
}

bool Compiler::is_in(const char elem, const std::unordered_set<char> us) {
    return us.find(elem) != us.end();
}
    
} //end namespace