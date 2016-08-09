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
const char Compiler::IF_CHAR = 'i';
const char Compiler::ELSE_CHAR = 'l';
const char Compiler::WHILE_CHAR = 'w';
const char Compiler::LOOP_CHAR = 'p';
const char Compiler::REPEAT_CHAR = 'r';
const char Compiler::UNTIL_CHAR = 'u';
const char Compiler::FOR_CHAR = 'f';
const char Compiler::DO_CHAR = 'd';
const char Compiler::BREAK_CHAR = 'b';
const std::string Compiler::ERR_LABEL("err_label");
const std::unordered_set<char> Compiler::BLOCK_ENDS({END_CHAR, ELSE_CHAR, UNTIL_CHAR});
    
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
    block(ERR_LABEL);
    if (m_input_stream.peek() != END_CHAR) {
        expected("End");
    }
    emit_line("return;");
}

void Compiler::block (const std::string exit_label) {
    while (!is_in(m_input_stream.peek(), BLOCK_ENDS)) {
        switch (m_input_stream.peek()) {
            case IF_CHAR:
                if_statement(exit_label);
                break;
            case WHILE_CHAR:
                while_statement();
                break;
            case LOOP_CHAR:
                loop_statement();
                break;
            case REPEAT_CHAR:
                repeat_statement();
                break;
            case FOR_CHAR:
                for_statement();
                break;
            case DO_CHAR:
                do_statement();
                break;
            case BREAK_CHAR:
                break_statement(exit_label);
                break;
            default:
                other();
                break;
        }
    }
}

void Compiler::if_statement(const std::string exit_label) {
    match(IF_CHAR);
    condition();
    const std::string label_one = new_label();
    std::string label_two = label_one;
    branch_on_not_cond(label_one);
    block(exit_label);
    if (m_input_stream.peek() == ELSE_CHAR) {
        match(ELSE_CHAR);
        label_two = new_label();
        jump(label_two);
        post_label(label_one);
        block(exit_label);
    }
    match(END_CHAR);
    post_label(label_two);
    
}

void Compiler::while_statement() {
    match(WHILE_CHAR);
    const std::string loop_start = new_label();
    const std::string loop_end = new_label();
    post_label(loop_start);
    condition();
    branch_on_not_cond(loop_end);
    block(loop_end);
    match(END_CHAR);
    jump(loop_start);
    post_label(loop_end);
}

void Compiler::loop_statement() {
    match(LOOP_CHAR);
    const std::string loop_start = new_label();
    const std::string loop_end = new_label();
    post_label(loop_start);
    block(loop_end);
    match(END_CHAR);
    jump(loop_start);
    post_label(loop_end);
}

void Compiler::repeat_statement() {
    match(REPEAT_CHAR);
    const std::string loop_start = new_label();
    const std::string loop_end = new_label();
    post_label(loop_start);
    block(loop_end);
    match(UNTIL_CHAR);
    condition();
    branch_on_not_cond(loop_start);
    post_label(loop_end);
}

void Compiler::for_statement() {
    match(FOR_CHAR);
    const std::string loop_start = new_label();
    const std::string loop_end = new_label();
    char counter_name = get_name();         //get name of loop counter
    match('=');
    expression();       //get initial value
    emit_line(std::string("cpu_variables['") + counter_name + "'] = cpu_registers.at(0);"); //initialize loop counter w/ initial value
    emit_line(std::string("--cpu_variables['") + counter_name + "'];");  //predecrement loop counter
    expression();       //get upper limit
    emit_line("cpu_stack.push(cpu_registers.at(0));"); //push upper limit to stack
    post_label(loop_start); //define L1
    emit_line(std::string("++cpu_variables['") + counter_name + "'];");    //increment the counter
    emit_line(std::string("cond = cpu_variables['") + counter_name + "'] > cpu_stack.top();"); //test if counter > upper limit
    branch_on_cond(loop_end); //leave loop if counter > upper limit
    block(loop_end);  //body of for statement
    match(END_CHAR);
    jump(loop_start); //go back to beginning of loop
    post_label(loop_end);
    emit_line("cpu_stack.pop();"); //clean upper limit off of stack
}

void Compiler::do_statement() {
    match(DO_CHAR);
    const std::string loop_start = new_label();
    const std::string loop_end = new_label();
    expression();       //get number of iterations
    post_label(loop_start);
    emit_line("cond = cpu_registers.at(0) <= 0;");     
    branch_on_cond(loop_end);
    emit_line("cpu_stack.push(cpu_registers.at(0));");
    block(loop_end);
    match(END_CHAR);
    emit_line("cpu_registers.at(0) = cpu_pop();");
    emit_line("--cpu_registers.at(0);");
    jump(loop_start);
    post_label(loop_end);
}

void Compiler::break_statement(const std::string exit_label) {
    match(BREAK_CHAR);
    if (exit_label == ERR_LABEL) {
        abort("No loop to break from.");
    } else {
        jump(exit_label);
    }
}

void Compiler::expression() {
    //dummy version
    emit_line("cpu_registers.at(0) = 1;");
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