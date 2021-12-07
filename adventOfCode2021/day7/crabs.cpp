// g++ -Wall -std=c++14 crabs.cpp -o crabs

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
//#include <set>
#include <vector>
#include <stdlib.h>

std::vector<std::string> getTokens(std::string& str){
    std::regex reg(",");
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

int main(){
    
    std::ifstream file("input.txt");
    std::string line;
    
    if(file.is_open()){
        
        std::vector<int> positions;
        int min = 0;
        int max = 0;
        
        while(std::getline(file, line)){ 
            std::vector<std::string> nums = getTokens(line);
            std::for_each(nums.begin(), nums.end(), [&](std::string& s){
                int num = atoi(s.c_str());
                positions.push_back(num);
                min = std::min(min, num);
                max = std::max(max, num);
            });
        }
        
        std::cout << "min: " << min << ", max: " << max << '\n';
        // if int& pos,
        // I get "error: binding 'const int' to reference of type 'int&' discards qualifiers"
        // ???
        int minSum = -1;
        int bestPos = -1;
        for(int pos = min; pos <= max; pos++){
            int sum = 0;
            //std::cout << pos << '\n';
            for(int p : positions){
                //sum += std::abs(p - pos);
                
                // for pt2, Gauss' formula is a big help!
                int diff = std::abs(p - pos);
                sum += (int)((diff * (diff+1)) / 2);
            }
            
            if(minSum == -1){
                minSum = sum;
                bestPos = pos;
            }else{
                if(sum < minSum){
                    minSum = sum;
                    bestPos = pos;
                }
            }
        }
        
        std::cout << "best align pos: " << bestPos << ", fuel total: " << minSum << '\n';
        
        file.close();
    }
    
    return 0;
}
