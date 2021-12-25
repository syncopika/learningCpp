// g++ -Wall -std=c++14 trickshot.cpp -o trickshot
#include <iostream>

struct target {
    int minX;
    int maxX;
    int minY;
    int maxY;   
};

bool fallsInRange(int x, int min, int max){
    return x >= min && x <= max;
}

int summation(int num){
    return (num*(num + 1))/2;
}

int checkVelocity(int velX, int velY, target& targetArea){
    // run through the simulation of having (x, y) be the initial velocity
    // and note the maxY, if higher than the current one.
    int currX = 0;
    int currY = 0;
    int maxY = 0;
    bool hitsTarget = false;
    while(currX < targetArea.maxX && currY > targetArea.minY){
        currX += velX;
        currY += velY;
        
        if(fallsInRange(currX, targetArea.minX, targetArea.maxX) && fallsInRange(currY, targetArea.minY, targetArea.maxY)){
            hitsTarget = true;
            break;
        }
        if(currY > maxY) maxY = currY;
        
        velY--;
        if(velX > 0) velX--;
    }
    
    if(hitsTarget){
        return maxY;
    }else{
        return -1;
    }
}

int main(){
    // not feeling like parsing this particular input file lol
    target targetArea{25, 67, -260, -200}; // targetArea{20, 30, -10, -5};
    
    int maxY = 0;
    
    int targetHitCount = 0;
    
    // try a bunch of velocities
    for(int x = 0; x <= targetArea.maxX; x++){
        // try to minimize number of x velocities we need to check
        if(summation(x) >= targetArea.minX){
            // I would like to know how to find the best range of y-values to check but am not sure how to currently
            // so I've resorted to guessing a suitable range
            for(int y = -800; y < 800; y++){
                int newMaxY = checkVelocity(x, y, targetArea);
                
                // part 2
                if(newMaxY > -1){
                    //std::cout << "hit target - (x:" << x << ", y: " << y << ")\n";
                    targetHitCount++;
                }
                
                if(newMaxY > -1 && newMaxY > maxY){
                    //std::cout << "found new max height for (x:" << x << ", y: " << y << ") => " << newMaxY << '\n';
                    maxY = newMaxY;
                }else if(newMaxY > -1 && newMaxY < maxY){
                    //std::cout << "next max height for (x:" << x << ", y: " << y << ") => " << maxY << '\n';
                    //goto done; // we're done (part 1)
                }
            }
        }
    }
    //done:
    
    std::cout << "the maximum y is: " << maxY << '\n';
    std::cout << "num velocities to hit target: " << targetHitCount << '\n';
    
    return 0;
}
