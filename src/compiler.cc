/*
    Implementation of the Compiler class.


*/

#include <cctype>       //character comparison functions
#include <stdexcept>
#include "compiler.hh"

namespace ds_compiler {
    
const size_t Compiler::NUM_REGISTERS = 8;
const char Compiler::ERR_CHAR = '\0';
const std::unordered_set<char> Compiler::ADD_OPS({'+', '-'});
const std::unordered_set<char> Compiler::MULT_OPS({'*', '/'});
    
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
    emit_line("#include <iostream>");
    emit_line("#include <string>");
    emit_line("#include <unordered_map>");
    
    //create a stack and vector for registers
    std::string stack_init = "static std::stack<int> cpu_stack;";
    std::string registers_init = "static std::vector<int> cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0);";
    std::string variables_init = "static std::unordered_map<char, int> cpu_variables(";
    emit_line(stack_init);
    emit_line(registers_init);
    emit_line(variables_init);
    
    //emit definition of a function for easier stack handling
    emit_line("int cpu_pop() {");
    emit_line("int val = cpu_stack.top();");
    emit_line("cpu_stack.pop();");
    emit_line("return val; }");
    
    //emit lines for int main() {
    emit_line("int main () {");
    
    
    
    
    
}

void Compiler::compile_end () const {
    
    //dump register contents
    emit_line("std::cout << \"Register contents\\n\";");
    emit_line(
        std::string("for (int i = 0; i < ") + std::to_string(NUM_REGISTERS) + "; ++i)"
        );
    emit_line("std::cout << std::string(\"Register \") << i << \": \" << cpu_registers.at(i) << '\\n';");
    
    //dump stack contents
    emit_line("std::cout << \"Stack contents (top to bottom)\\n\";");
    emit_line("while (!cpu_stack.empty()) {");
    emit_line("std::cout << cpu_stack.top() << '\\n';");
    emit_line("cpu_stack.pop();}");
    
    //close main()
    emit_line("return 0;");
    emit_line("}");
    
}

void Compiler::expression () const {
    
    if (is_in(is().peek(), ADD_OPS)) {
        emit_line("cpu_registers.at(0) = 0;");  //support unary +/- by inserting 0 before the op
    } else {
        term();
    }
    
    while (is_in(is().peek(), ADD_OPS)) {
        emit_line("cpu_stack.push(cpu_registers.at(0));");
        switch (is().peek()) {
            case '+':
                add();
                break;
            case '-':
                subtract();
                break;
            default:
                expected("Add op");
        }
    }
}
    
void Compiler::term () const {
    factor();
    while (is_in(is().peek(), MULT_OPS)) {
        emit_line("cpu_stack.push(cpu_registers.at(0));");
        switch (is().peek()) {
            case '*':
                multiply();
                break;
            case '/':
                divide();
                break;
            default:
                expected("Mult op");
        }
    }
}
    
void Compiler::factor () const {
    
    if (is().peek() == '(') {
        match('(');
        expression();
        match(')');
    } else if (std::isalpha(is().peek())) {
        ident();
    } else {
        char expr = get_num();
        if (expr != ERR_CHAR) {
            emit_line(std::string("cpu_registers.at(0) = ") + expr + ";");
        } 
    }
}

//handles function calls/variables
void Compiler::ident () const {
    char name = get_name();

    if (name != ERR_CHAR) { 
        if (is().peek() == '(') {//function call
            match('(');
            match(')');
            emit_line(std::string(1, name) + "();");
        } else {
            emit_line(std::string("cpu_registers.at(0) = cpu_variables.at(\'") + name + "\');");
        }
    } 
}

void Compiler::add () const {
    match('+');
    term();
    emit_line("cpu_registers.at(0) = cpu_stack.top() + cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::subtract () const {
    match('-');
    term();
    emit_line("cpu_registers.at(0) = cpu_stack.top() - cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::multiply() const {
    match('*');
    factor();
    emit_line("cpu_registers.at(0) = cpu_stack.top() * cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::divide() const {
    match('/');
    factor();
    emit_line("cpu_registers.at(0) = cpu_stack.top() / cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
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

bool Compiler::is_in(const char elem, const std::unordered_set<char> us) {
    return us.find(elem) != us.end();
}

//helper members to allow using stream syntax with *is, *os

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