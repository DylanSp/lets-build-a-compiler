/*
    Utility functions and lexing.





*/

#ifndef CRADLE_CC
#define CRADLE_CC

#include <iostream>
#include <string>
#include <cctype>

//empty namespace to ensure uniqueness when linking
namespace { 


const char ERR_CHAR = '\0';
char lookahead;


//compiler utility functions
void report_error(const std::string err, std::ostream &os) {
    
    os << '\n';
    os << "Error: " << err << '\n';
    
}

void abort(const std::string err, std::ostream &os) {
    
    report_error(err, os);
    //TODO - halt execution
}

void expected(const std::string expect, std::ostream &os) {
    
    abort(expect + " expected.\n", os);
}

//overload to handle single characters; 
//prevents having to construct a string whenever calling expected()
void expected(const char c, std::ostream &os) {
    expected(std::string(1, c), os);
}
    
//lexing

//get character from input, advance input
void get_char(std::istream &is) {
    lookahead = is.get();
}

void match(const char c, std::istream &is, std::ostream &os) {
    
    if (is.peek() == c) {
        get_char(is);
    } else {
        expected(c, os);
    }
    
    
}

// gets a valid identifier
//TODO - decide whether this is passed c as input or examines lookahead

char get_name (const char c, std::istream &is, std::ostream &os) {
    if (!std::isalpha(c)) {
        expected("Name", os);
        return ERR_CHAR;
    } else {
        
        //placeholder
        return 'N'; 
    }
    
}

//gets a number
char get_num (std::istream &is, std::ostream &os) {
    
    
    //placeholder
    return 'T'; 
}


//output a string 
void emit (std::string s, std::ostream &os) {
    os << s;
}

//output a string with newline 
void emit_line (std::string s, std::ostream &os) {
    emit(s, os);
    emit("\n", os);
}
    

    
    
} //end namespace

#endif