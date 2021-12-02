// g++ -Wall -std=c++14 sonar_sweep.cpp -o sonar_sweep

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdlib.h> // atoi

void part1(){
    std::ifstream file("input.txt");
    
    std::string line;
    int numIncrease = 0;
    int prev = -1;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            int currNum = atoi(line.c_str());
            if(prev != -1){
                if(currNum > prev){
                    numIncrease++;
                }
            }
            prev = currNum;
        }
        
        file.close();
    }
    
    std::cout << "number of increases: " << numIncrease << '\n';
}

void part2(){
    // three-measurement sliding window
    std::ifstream file("input.txt");
    
    std::string line;
    std::vector<int> data;

    if(file.is_open()){
        while(std::getline(file, line)){
            data.push_back(atoi(line.c_str()));
        }
        file.close();
    }
    //std::vector<int> data{199, 200, 208, 210, 200, 207, 240, 260, 260, 263};
    
    int numIncrease = 0;
    int prevSum = -1;
    for(size_t i = 0; i <= data.size()-3; i++){
        if(i+3 > data.size()){
            break;
        }
        
        int sum = 0;
        for(int j = i; j < (int)i+3; j++){
            sum += data[j];
        }
        
        if(prevSum != -1){
            if(sum > prevSum) numIncrease++;
        }
        
        prevSum = sum;
    }
    
    std::cout << "number of three-measurement increases: " << numIncrease << '\n';    
}

int main(void){
    part1();
    part2();
}