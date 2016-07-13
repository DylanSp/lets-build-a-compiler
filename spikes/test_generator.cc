#include <iostream>
#include <fstream>
#include <map>
#include "compiler.hh"

//primary test case - GetsCorrectResults
//checks variable value(s)
void generate_correct_functionality_test (const std::string class_name, 
                                          std::map<char, int> expected_values, 
                                          std::ofstream& ofs) {
    std::string test_class_name = class_name + "Class";
    ofs << "TEST_F(" << test_class_name << ", GetsCorrectResults ) {" << '\n';
    for (auto test : expected_values) {
        ofs << "EXPECT_EQ(" << test.second << ", tested_object.get_variable('" << test.first << "'));" << '\n';
    }
    ofs << "}" << '\n';
    
}


//secondary test case - EmptiesStack
//checks that stack is empty after running
void generate_empties_stack_test (const std::string class_name, std::ofstream& ofs) {
    std::string test_class_name = class_name + "Class";
    ofs << "TEST_F(" << test_class_name << ", EmptiesStack ) {" << '\n'; 
    ofs << "EXPECT_TRUE(tested_object.is_stack_empty());" << '\n';
    ofs << "}" << '\n';
}

void generate_test_fixture (const std::string class_name, std::ofstream& ofs) {
    std::string test_class_name = class_name + "Class";
    ofs << "#include \"gtest/gtest.h\"" << '\n';
    ofs << "#include \"" << class_name << ".cc\"" << '\n';
    ofs << "class " << test_class_name << " : public ::testing::Test {" << '\n';
    ofs << "public:" << '\n';
    
    //define ctor, with run() call 
    ofs << class_name << " tested_object;" << '\n';
    ofs << test_class_name << " () : tested_object() {" << '\n';
    ofs << "tested_object.run();" << '\n';
    ofs << "}" << '\n';
    
    //close test fixture definition
    ofs << "};" << '\n';
}

void create_testable_class (const std::string class_name,
                            const std::vector<std::string> program,
                            std::ofstream& ofs) {
    ds_compiler::Compiler my_compiler(ofs);
    my_compiler.compile_full(program, class_name);
}

struct test_input_params {
    std::string class_name;
    std::vector<std::string> program_source;
    std::map<char, int> expected_values;
};

std::vector<test_input_params> initialize_test_params () {
    std::vector<test_input_params> test_params {
        {"EmptyProgram", {}, {}},
        {"SingleConstant", {1, "x=1"}, {{'X', 1}}},
        {"SimpleAddition", {1, "x=1+1"}, {{'X', 2}}},
        {"SimpleSubtraction", {1, "x=4-1"}, {{'X',3}}},
        {"SimpleMultiplication", {1, "x=2*2"}, {{'X',4}}},
        {"SimpleDivision", {1, "x=4/2"}, {{'X',2}}},
        {"MultipleAddOps", {1, "x=1+1-3"}, {{'X',-1}}},
        {"MultipleMultOps", {1, "x=4*5/2"}, {{'X',10}}},
        {"SimpleUnaryMinus", {1, "x=-2"}, {{'X',-2}}},
        {"SimpleParens", {1, "x=(1+2)"}, {{'X',3}}},
        {"ParensPrecedence", {1, "x=2*(3+1)"}, {{'X',8}}},
        {"AddUnaryMinus", {1, "x=-1+2"}, {{'X',1}}},
        {"SubtractUnaryMinus", {1, "x=-1-1"}, {{'X',-2}}},
        {"MultiplyUnaryMinus", {1, "x=-1*3"}, {{'X',-3}}},
        {"DivideUnaryMinus", {1, "x=-4/1"}, {{'X',-4}}}
    };
    
    return test_params;
}

int main () {
    const std::string PROJ_ROOT("/home/ubuntu/workspace/");
    const std::string CLASS_DIR("test/generated/");
    const std::string TEST_DIR("test/generated/");
    
    std::vector<test_input_params> test_params = initialize_test_params();
    
    for (auto i : test_params) {
        std::string class_file_name(PROJ_ROOT + CLASS_DIR + i.class_name + ".cc");
        std::ofstream class_ofs(class_file_name, std::ofstream::out);
        
        create_testable_class(i.class_name, i.program_source, class_ofs);

        std::string test_file_name(PROJ_ROOT + TEST_DIR + i.class_name + "Test.cc");
        std::ofstream test_ofs(test_file_name, std::ofstream::out);
        
        generate_test_fixture(i.class_name, test_ofs);
        generate_correct_functionality_test(i.class_name, i.expected_values, test_ofs);
        generate_empties_stack_test(i.class_name, test_ofs);
    }
    
    return 0;
}
