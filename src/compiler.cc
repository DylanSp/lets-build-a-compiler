/*
    Implementation of the Compiler class.


*/

#include <cctype>       //character comparison functions
#include <stdexcept>
#include "compiler.hh"

namespace ds_compiler {
    
const size_t Compiler::NUM_REGISTERS = 8;
const char Compiler::ERR_CHAR = '\0';
const std::unordered_set<char> ADD_OPS({'+', '-'});
const std::unordered_set<char> MULT_OPS();
    
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
    try {
        expression();
    } catch (std::exception &ex) {
        std::cerr << ex.what() << '\n';
    }
}
    
void Compiler::compile_full () const {
    
    compile_start();
    compile_intermediate();
    compile_end();
    
}

void Compiler::compile_start () const {
    
    //emit lines for necessary includes
    emit_line("#include <stack>");
    emit_line("#include <vector>");
    
    //emit lines for int main() {
    emit_line("int main () {");
    
    //create a stack and vector for registers
    std::string stack_init = "std::stack<int> cpu_stack;";
    std::string registers_init = "std::vector<int> cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0);";
    
    emit_line(stack_init);
    emit_line(registers_init);
    
}

void Compiler::compile_end () const {
    
    //emit lines for closing main
    emit_line("return 0;");
    emit_line("}");
    
}
    

//cradle methods

void Compiler::report_error(const std::string err) const {
    
    os() << '\n';
    os() << "Error: " << err << '\n';
    
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

//checks if next character matches; if so, consume that character
void Compiler::match(const char c) const {
    
    if (is().peek() == c) {
        is().get(); 
    } else {
        expected(c);
    }
}

void Compiler::expression () const {
    term();
    emit_line(
        "cpu_stack.at(1) = cpu_stack.at(0);"
    );
    switch (is().peek()) {
        case '+':
            add();
            break;
        case '-':
            subtract();
            break;
        default:
            expected("Addop");
    }
}
    
void Compiler::term () const {
    char expr = get_num();
    
    if (expr != ERR_CHAR) {
        emit_line(
            std::string("cpu_registers.at(0) = ") + expr + ";"
        );
    }
}

// gets a valid identifier from input stream
char Compiler::get_name () const {
    if (!std::isalpha(is().peek())) {
        expected("Name");
        return ERR_CHAR;
    } else {
        return std::toupper(is().get());
    }
    
}

//gets a number
char Compiler::get_num () const {
    if (!std::isdigit(is().peek())) {
        expected("Integer");
        return ERR_CHAR;
    } else {
        return is().get();
    }
}


//output a string 
void Compiler::emit (std::string s) const {
    os() << s;
}

//output a string with newline 
void Compiler::emit_line (std::string s) const {
    emit(s);
    emit("\n");
}
    


//helper members to allow using stream syntax with *is, *os

void Compiler::add () const {
    match('+');
    term();
    emit_line(
        std::string("cpu_registers.at(0) = cpu_registers.at(1) + cpu_registers.at(0);")
    );
}

void Compiler::subtract () const {
    match('-');
    term();
    emit_line(
        std::string("cpu_registers.at(0) = cpu_registers.at(1) - cpu_registers.at(0);")
    );
}

static bool is_in(const std::unordered_set<char> us, const char elem) {
    return us.find(elem) != us.end();
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