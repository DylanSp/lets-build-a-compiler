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
        {"BoolAndNot", {"t&!f"}, {{"0", 1}}},
        {"BoolNotAnd", {"!t&t"}, {{"0", 0}}},
        {"BoolNotOr", {"!f|f"}, {{"0", 1}}},
        {"BoolOrNot", {"t|!f"}, {{"0", 1}}},
        {"BoolAndOrPrecedence", {"t&t|f&f"}, {{"0", 1}}},
        {"BoolTripleOr", {"f|f|t"}, {{"0", 1}}},
        {"BoolTripleAnd", {"t&t&f"}, {{"0", 0}}},
        {"BoolDoubleNot", {"!!t"}, {{"0", 1}}},
    };
    
    return test_params;
}



int main () {
    std::vector<test_input_params> test_params = initialize_test_params();
    
    for (auto params : test_params) {
        YAML::Emitter spec;
        spec << YAML::BeginMap;
        
        spec << YAML::Key << "class_name";
        spec << YAML::Value << params.class_name;
        
        spec << YAML::Key << "program_source";
        spec << YAML::Value << YAML::BeginSeq;
        for (auto line : params.program_source) {
            spec << YAML::DoubleQuoted << line;
        }
        spec << YAML::EndSeq;
        
        spec << YAML::Key << "expected_values";
        spec << YAML::Value << YAML::BeginMap;
        for (auto value_pair : params.expected_values) {
            spec << YAML::Key << value_pair.first;
            spec << YAML::Value << value_pair.second;
        }
        spec << YAML::EndMap;
        
        spec << YAML::EndMap;
        
        std::ofstream ofs("test/new_specs/" + params.class_name + ".yml");
        ofs << spec.c_str();
    }
    
    return 0;
}