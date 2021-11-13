#include "Vec3.h"

#include <iostream>
#include <stdexcept>
#include <cassert>

int main(void){
    // test Vec3 class
    
    Vec3 test(0,5,1);
    Vec3 test2{2,4,10};
    
    assert(test[0] == 0);
    assert(test[1] == 5);
    assert(test[2] == 1);
    //std::cout << test[0] << ", " << test[1] << ", " << test[2] << '\n';
    
    test += test2;
    assert(test2[0] == 2);
    assert(test2[1] == 4);
    assert(test2[2] == 10);
    //std::cout << test[0] << ", " << test[1] << ", " << test[2] << '\n';
    
    // test exceptions
    try {
        double x = test[3]; // 3 is not a valid index
        std::cout << x << '\n';
    }
    catch(const std::invalid_argument& err){
        std::cout << err.what() << '\n';
    }
    
    Vec3 unit = unit_vector(test);
    std::cout << unit << '\n';
    
    return 0;
}