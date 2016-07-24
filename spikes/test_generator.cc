#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "compiler.hh"
#include "yaml-cpp/yaml.h"

//primary test case - GetsCorrectResults
//checks variable value(s)
void generate_correct_functionality_test (const std::string class_name, 
                                          std::map<std::string, int> expected_values, 
                                          std::ofstream& ofs) {
    std::string test_class_name = class_name + "Class";
    ofs << "TEST_F(" << test_class_name << ", GetsCorrectResults ) {" << '\n';
    for (auto test : expected_values) {
        ofs << "EXPECT_EQ(" << test.second << ", tested_object.get_variable(\"" << test.first << "\"));" << '\n';
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
    std::map<std::string, int> expected_values;
};

std::vector<test_input_params> initialize_test_params () {
    std::vector<test_input_params> test_params {
        {"EmptyProgram", {}, {}},
        {"SingleConstant", {"x=1"}, {{"X", 1}}},
        {"SimpleAddition", {"x=1+1"}, {{"X", 2}}},
        {"SimpleSubtraction", {"x=4-1"}, {{"X",3}}},
        {"SimpleMultiplication", {"x=2*2"}, {{"X",4}}},
        {"SimpleDivision", {"x=4/2"}, {{"X",2}}},
        {"MultipleAddOps", {"x=1+1-3"}, {{"X",-1}}},
        {"MultipleMultOps", {"x=4*5/2"}, {{"X",10}}},
        {"SimpleUnaryMinus", {"x=-2"}, {{"X",-2}}},
        {"SimpleParens", {"x=(1+2)"}, {{"X",3}}},
        {"ParensPrecedence", {"x=2*(3+1)"}, {{"X",8}}},
        {"AddUnaryMinus", {"x=-1+2"}, {{"X",1}}},
        {"SubtractUnaryMinus", {"x=-1-1"}, {{"X",-2}}},
        {"MultiplyUnaryMinus", {"x=-1*3"}, {{"X",-3}}},
        {"DivideUnaryMinus", {"x=-4/1"}, {{"X",-4}}},
        {"SimpleVariableUse", {"x=1", "y=x"}, {{"X",1}, {"Y", 1}}},
        {"VariableAddition", {"x=2", "y=x+1"}, {{"X",2}, {"Y", 3}}},
        {"VariableSubtraction", {"x=3", "y=x-2"}, {{"X",3}, {"Y", 1}}},
        {"VariableMultiplication", {"x=4", "y=5*x"}, {{"X",4}, {"Y", 20}}},
        {"VariableDivision", {"x=6", "y=x/2"}, {{"X",6}, {"Y", 3}}},
        {"DivisionByVariable", {"x=4", "y=8/x"}, {{"X",4}, {"Y", 2}}},
        {"AddMultPrecedence", {"x=1+2*3"}, {{"X", 7}}},
        {"SubMultPrecedence", {"x=4-5*6"}, {{"X", -26}}},
        {"AddDivPrecedence", {"x=7+8/2"}, {{"X", 11}}},
        {"SubDivPrecedence", {"x=4-6/3"}, {{"X", 2}}},
        {"SimpleFunctionCall", {"x=f()"}, {}},
        {"SimpleMultiCharVariable", {"test=1"}, {{"TEST", 1}}},
        {"SimpleMultiDigitNum", {"x=23"}, {{"X", 23}}},
        {"MultiCharMultiDigit", {"test=34"}, {{"TEST", 34}}},
        {"SimpleWhitespace", {"test = 5 + 6"}, {{"TEST", 11}}},
        {"TabWhitespace", {"test\t=\t 7 + 8"}, {{"TEST", 15}}},
        {"TrailingZeroes", {"test = 10 + 2"}, {{"TEST", 12}}},
        {"TheKitchenSink", {"intermediate = 12+34-56*8/64","result = -\t(intermediate+123)*456"}, 
                           {{"INTERMEDIATE", 39}, {"RESULT", -73872}}},
    };
    
    return test_params;
}

int main (int argc, char *argv[]) {
    
    //validate arguments
    if (argc < 2) {
        std::cerr << "Needs at least one argument." << '\n';
        return 1;
    }
    
    const std::string PROJ_ROOT("/home/ubuntu/workspace/");
    //const std::string SPEC_DIR("test/specs/");
    const std::string SPEC_DIR("");
    
    std::vector<test_input_params> test_params;
    for (int i = 1; i < argc; ++i) {
        std::string spec_file_name(PROJ_ROOT + SPEC_DIR + argv[i]);
        
        //DEBUG
        std::cout << "Loading " << spec_file_name << '\n';
        
        YAML::Node test_spec = YAML::LoadFile(spec_file_name);
        
        test_input_params params;    
        
        params.class_name = test_spec["class_name"].as<std::string>();
        
        for (auto line : test_spec["program_source"]) {
            params.program_source.push_back(line.as<std::string>());
        }
        
        for (auto value_pair : test_spec["expected_values"]) {
            params.expected_values[value_pair.first.as<std::string>()] = value_pair.second.as<int>();
        }

        test_params.push_back(params);
    }
    
    const std::string CLASS_DIR("test/generated/");
    const std::string TEST_DIR("test/generated/");
    
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
