/*
    Implementation of the Compiler class.


*/

#include <cctype>       //character comparison functions
#include <stdexcept>
#include <assert.h>
#include "compiler.hh"

namespace ds_compiler {
    
const size_t Compiler::NUM_REGISTERS = 8;
const char Compiler::ERR_CHAR = '\0';
const std::unordered_set<char> Compiler::ADD_OPS({'+', '-'});
const std::unordered_set<char> Compiler::MULT_OPS({'*', '/'});
    
//constructors
Compiler::Compiler (std::ostream& output) 
    : m_input_stream (std::ios::in|std::ios::out), m_output_stream(output)
{
    
}



void Compiler::compile_intermediate (const std::string input_line) {
    
    //clear contents and error flags on m_input_stream
    m_input_stream.str("");
    m_input_stream.clear();
    
    m_input_stream << input_line;
    
    try {
        start_symbol();
    } catch (std::exception &ex) {
        std::cerr << ex.what() << '\n';
        throw std::runtime_error("Compilation failed.\n");
    }
}
    
void Compiler::compile_full (const std::vector<std::string> source, const std::string class_name) {
    
    compile_start(class_name);
    for (auto line : source) {
        compile_intermediate(line);
    }    
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
    //TODO - are these dumps necessary?
    
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


void Compiler::start_symbol () {
    assignment();
}

void Compiler::assignment () {
    char name = get_name();
    match('=');
    expression();
    emit_line(std::string("cpu_variables['") + name + "'] = cpu_registers.at(0);");
}

void Compiler::expression () {
    
    if (is_in(m_input_stream.peek(), ADD_OPS)) {
        emit_line("cpu_registers.at(0) = 0;");  //support unary +/- by inserting 0 before the op
    } else {
        term();
    }
    
    while (is_in(m_input_stream.peek(), ADD_OPS)) {
        emit_line("cpu_stack.push(cpu_registers.at(0));");
        switch (m_input_stream.peek()) {
            case '+':
                add();
                break;
            case '-':
                subtract();
                break;
            default: //should never be reached!
                assert(false);
        }
    }
}
    
void Compiler::term () {
    factor();
    while (is_in(m_input_stream.peek(), MULT_OPS)) {
        emit_line("cpu_stack.push(cpu_registers.at(0));");
        switch (m_input_stream.peek()) {
            case '*':
                multiply();
                break;
            case '/':
                divide();
                break;
            default: //should never be reached!
                assert(false);
        }
    }
}
    
void Compiler::factor () {
    
    if (m_input_stream.peek() == '(') {
        match('(');
        expression();
        match(')');
    } else if (std::isalpha(m_input_stream.peek())) {
        ident();
    } else {
        char expr = get_num();
        if (expr != ERR_CHAR) {
            emit_line(std::string("cpu_registers.at(0) = ") + expr + ";");
        } 
    }
}

//handles function calls/variables
void Compiler::ident () {
    char name = get_name();

    if (name != ERR_CHAR) { 
        if (m_input_stream.peek() == '(') {//function call
            match('(');
            match(')');
            define_function(name);
            emit_line(std::string(1, name) + "();");
        } else {
            emit_line(std::string("cpu_registers.at(0) = cpu_variables.at(\'") + name + "\');");
        }
    } 
}

void Compiler::add () {
    match('+');
    term();
    emit_line("cpu_registers.at(0) = cpu_stack.top() + cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::subtract () {
    match('-');
    term();
    emit_line("cpu_registers.at(0) = cpu_stack.top() - cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::multiply() {
    match('*');
    factor();
    emit_line("cpu_registers.at(0) = cpu_stack.top() * cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

void Compiler::divide() {
    match('/');
    factor();
    emit_line("cpu_registers.at(0) = cpu_stack.top() / cpu_registers.at(0);");
    emit_line("cpu_stack.pop();");
}

//defines an empty lambda so output compiles
void Compiler::define_function (char ident) const {
    emit_line(std::string("auto ") + ident + " = [](){};");
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

//checks if next character matches; if so, consume that character
void Compiler::match(const char c) {
    
    if (m_input_stream.peek() == c) {
        m_input_stream.get(); 
    } else {
        expected(c);
    }
}

// gets a valid identifier from input stream
char Compiler::get_name () {
    if (!std::isalpha(m_input_stream.peek())) {
        expected("Name");
        return ERR_CHAR;
    } else {
        return std::toupper(m_input_stream.get());
    }
    
}

//gets a number
char Compiler::get_num () {
    if (!std::isdigit(m_input_stream.peek())) {
        expected("Integer");
        return ERR_CHAR;
    } else {
        return m_input_stream.get();
    }
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