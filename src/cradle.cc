/*
    Utility functions and lexing.





*/

#ifndef CRADLE_CC
#define CRADLE_CC

#include <iostream>
#include <string>
#include <cctype>

namespace /*ds_compiler*/ {


const char ERR_CHAR = '\0';
  
std::istream *is_ptr = &std::cin;
std::ostream *os_ptr = &std::cout;

char lookahead;

//utility function; allows use of stream syntax with pointers to streams
std::istream& is() { 
    return *is_ptr;
}

std::ostream& os() { 
    return *os_ptr;
}


//compiler utility functions
void report_error(const std::string err) {
    
    os() << '\n';
    os() << "Error: " << err << '\n';
    
}

void abort(const std::string err) {
    
    report_error(err);
    //TODO - halt execution
}

void expected(const std::string expect) {
    
    abort(expect + " expected.\n");
}

void expected(const char c) {
    expected(std::string(1, c));
}
    
//lexing

//get character from input, advance input
void get_char() {
    lookahead = is().get();
}

void match(const char c) {
    
    if (is().peek() == c) {
        get_char();
    } else {
        expected(c);
    }
    
    
}

// gets a valid identifier
char get_name (const char c) {
    if (!std::isalpha(c)) {
        expected("Name");
        return ERR_CHAR;
    } else {
        
    }
    
}

//gets a number
char get_num () {
    
}


//output a string 
void emit (std::string s) {
    os() << s;
}

//output a string with newline 
void emit_line (std::string s) {
    emit(s);
    emit("\n");
}
    

    
    
} //end namespace

#endif