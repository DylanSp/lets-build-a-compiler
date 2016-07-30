//testing how to use yaml-cpp

#include <string>
#include "yaml-cpp/yaml.h"
#include <iostream>

int main () {
    
    // --- SingleConstant ---
    {
        YAML::Node single_constant_spec = YAML::LoadFile("test/specs/SingleConstant.yml");

        std::string class_name = single_constant_spec["class_name"].as<std::string>();
        std::cout << "class_name: " << class_name << '\n';
        std::cout << '\n';
    
        std::cout << "program_source: " << '\n';
        for (int i = 0; i < 1; ++i) {
            std::cout << single_constant_spec["program_source"][i].as<std::string>() << '\n';
        }
        std::cout << '\n';
    
        std::cout << "expected_values: " << '\n';
    
        //directly access value, given the key
        std::cout << single_constant_spec["expected_values"]["X"].as<int>() << '\n';
    
        //get both key and value
        for (auto i : single_constant_spec["expected_values"]) {
            std::cout << i.first.as<char>() << " = " << i.second.as<int>() << '\n';
        }
    }
    
    
    // --- KitchenSink ---
    {
        YAML::Node kitchen_sink_spec = YAML::LoadFile("test/specs/KitchenSink.yml");

        std::string class_name = kitchen_sink_spec["class_name"].as<std::string>();
        std::cout << "class_name: " << class_name << '\n';
        std::cout << '\n';
    
        std::cout << "program_source: " << '\n';
        for (int i = 0; i < 2; ++i) {
            std::cout << kitchen_sink_spec["program_source"][i].as<std::string>() << '\n';
        }
        
        for (auto j : kitchen_sink_spec["program_source"]) {
            std::cout << j.as<std::string>() << '\n';
        }
        std::cout << '\n';
        
        std::cout << "expected_values: " << '\n';
    
        //get both key and value
        for (auto i : kitchen_sink_spec["expected_values"]) {
            std::cout << i.first.as<std::string>() << " = " << i.second.as<int>() << '\n';
        }
    }
    
    
    
    
    
    return 0;
}