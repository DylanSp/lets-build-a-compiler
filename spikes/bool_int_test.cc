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
    };
    
    std::cout << tests << '\n';
    
    
    return 0;
}