#include "yaml-cpp/yaml.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>


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



int main () {
    std::vector<test_input_params> test_params = initialize_test_params();
    
    for (auto params : test_params) {
        YAML::Node spec;
        
        spec["class_name"] = params.class_name;
        
        for (auto line : params.program_source) {
            spec["program_source"].push_back(line);
        }
        
        for (auto value_pair : params.expected_values) {
            spec["expected_values"][value_pair.first] = value_pair.second;
        }
        
        std::ofstream ofs("test/new_specs/" + params.class_name + ".yml");
        ofs << spec;
    }
    
    return 0;
}