#include "Vec3.h"

#include <iostream>
#include <stdexcept>

int main(void){
    // test Vec3 class
    
    Vec3 test(0,5,1);
    Vec3 test2{2,4,10};
    std::cout << test[0] << ", " << test[1] << ", " << test[2] << '\n';
    
    test += test2;
    std::cout << test[0] << ", " << test[1] << ", " << test[2] << '\n';
    
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