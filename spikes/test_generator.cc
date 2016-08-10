#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdexcept>
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
        if (std::isalpha(test.first.at(0))) {
            ofs << "EXPECT_EQ(" << test.second << ", " << "tested_object.get_variable(\"" << test.first << "\"));";
        } else if (std::stoi(test.first) >= 0 
                    && std::stoul(test.first) < ds_compiler::Compiler::NUM_REGISTERS) {
            ofs << "EXPECT_EQ(" << test.second << ", " << "tested_object.get_register(" << test.first << "));";
        } else {
            std::cerr << "Error: key for expected_values wasn't a variable name or register index." << '\n';
            throw std::runtime_error("Test generation failed.\n");
        }
        ofs << '\n';
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

//arguments - path (from project root) to .yml test spec
int main (int argc, char *argv[]) {
    
    //validate arguments
    if (argc < 2) {
        std::cerr << "Needs at least one argument." << '\n';
        return 1;
    }
    
    const std::string PROJ_ROOT("/home/ubuntu/workspace/");
    
    std::vector<test_input_params> test_params;
    for (int i = 1; i < argc; ++i) {
        std::string spec_file_name(PROJ_ROOT + argv[i]);
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
