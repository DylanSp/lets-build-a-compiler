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
const char Compiler::END_CHAR = 'e';
const std::unordered_set<char> Compiler::BLOCK_ENDS({END_CHAR, ELSE_CHAR});
const char Compiler::IF_CHAR = 'i';
const char Compiler::ELSE_CHAR = 'l';
const char Compiler::WHILE_CHAR = 'w';
const char Compiler::LOOP_CHAR = 'p';
    
//constructors
Compiler::Compiler (std::ostream& output) 
    : label_count(0), m_input_stream (std::ios::in|std::ios::out), m_output_stream(output) 
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
    emit_line("bool cond;");
}

void Compiler::define_constructor(const std::string class_name) const {
    emit_line(class_name + "() ");
    emit_line(": cpu_stack()");
    emit_line(", cpu_registers(" + std::to_string(NUM_REGISTERS) + ", 0)");
    emit_line(", cpu_variables()");
    emit_line(", cond(false)");
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
    program();
}

void Compiler::program () {
    block();
    if (m_input_stream.peek() != END_CHAR) {
        expected("End");
    }
    emit_line("return;");
}

void Compiler::block () {
    while (!is_in(m_input_stream.peek(), BLOCK_ENDS)) {
        switch (m_input_stream.peek()) {
            case IF_CHAR:
                parse_if();
                break;
            case WHILE_CHAR:
                parse_while();
                break;
            case LOOP_CHAR:
                parse_loop();
                break;
            default:
                other();
                break;
        }
    }
}

void Compiler::parse_if() {
    match(IF_CHAR);
    condition();
    std::string label_one = new_label();
    std::string label_two = label_one;
    branch_on_not_cond(label_one);
    block();
    if (m_input_stream.peek() == ELSE_CHAR) {
        match(ELSE_CHAR);
        label_two = new_label();
        jump(label_two);
        post_label(label_one);
        block();
    }
    match(END_CHAR);
    post_label(label_two);
    
}

void Compiler::parse_while() {
    match(WHILE_CHAR);
    std::string label_one = new_label();
    std::string label_two = new_label();
    post_label(label_one);
    condition();
    branch_on_not_cond(label_two);
    block();
    match(END_CHAR);
    jump(label_one);
    post_label(label_two);
}

void Compiler::parse_loop() {
    match(LOOP_CHAR);
    const std::string label = new_label();
    post_label(label);
    block();
    match(END_CHAR);
    jump(label);
}

void Compiler::condition() {
    //dummy version
    emit_line("cond = true;");
}

//equivalent of assembly's BNE
void Compiler::branch_on_cond(const std::string label) {
    emit_line({"if (cond) { goto " + label + "; }"});
}

//equivalent of assembly's BEQ
void Compiler::branch_on_not_cond(const std::string label) {
    emit_line({"if (!cond) { goto " + label + "; }"});
}

//equivalent of assembly's BRA, JMP
void Compiler::jump (const std::string label) {
    emit_line({"goto " + label + ";"});
}

void Compiler::other () {
    emit_line(std::string("std::cout << \"") + get_name() + "\" << '\\n';");
}


//label handling
std::string Compiler::new_label() {
    std::string label{'L' + std::to_string(label_count)};
    ++label_count;
    return label;
}

void Compiler::post_label(const std::string label) const {
    emit_line({label + ":"});
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