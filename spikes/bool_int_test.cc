#include <iostream>
#include <map>
#include "prettyprint.hpp"

int main () {
    
    std::map<char, int> tests {
        {'A', 1 || 1},      // 1
        {'B', !1 != !1},    // 0
        {'C', !1 != !0},    // 1
        {'D', !false},      // 1
        {'E', !1},          // 0
        {'F', 1 | 0},       // 1
        {'G', 0 | 0},       // 0
        {'H', 0 & 1},       // 0
        {'I', 1 & 1},       // 1
        {'J', ~1},          // -2, implementation-defined
        {'K', ~0},          // -1, implementation-defined
    };
    
    std::cout << tests << '\n';
    
    bool b = true;
    int i = b;
    std::cout << "b: " << b << '\n';
    std::cout << "i: " << i << '\n';
    
    int j = 1;
    bool c = j;
    
    std::cout << "j: " << j << '\n';
    std::cout << "c: " << c << '\n';
    
    bool b2 = false;
    int i2 = b2;
    std::cout << "b2: " << b2 << '\n';
    std::cout << "i2: " << i2 << '\n';
    
    int j2 = 0;
    bool c2 = j2;
    
    std::cout << "j2: " << j2 << '\n';
    std::cout << "c2: " << c2 << '\n';
    
    
    return 0;
}